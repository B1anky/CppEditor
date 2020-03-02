/**
MIT License

Copyright (c) 2019 Brett Sackstein

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "AnimatedHoverScrollBar.h"
#include <QMouseEvent>

#include <QDebug>

AnimatedHoverScrollBar::AnimatedHoverScrollBar(const QColor& defaultColor, const QColor& hoverColor,
                                               const QColor& pressedColor, const QColor& backgroundColor,
                                               const QColor& backgroundBorderColor, unsigned int borderWidth, QWidget* parent) :
    QScrollBar(parent),
    m_transition(nullptr),
    m_duration(250),
    m_heightMargin(0),
    m_widthMargin(0),
    m_borderWidth(borderWidth),
    m_defaultColor(defaultColor),
    m_hoverColor(hoverColor),
    m_pressedColor(pressedColor),
    m_backgroundColor(backgroundColor),
    m_backgroundBorderColor(backgroundBorderColor),
    m_currentColor(defaultColor),
    m_parentTable(qobject_cast<QTableWidget*>(parent)), //If we aren't parenting it to a QTableWidget, it's still fine, this will be a nullptr
    m_dragging(false),
    m_clickOffset(0),
    m_pressValue(0)
{

    m_transition = new QPropertyAnimation(this, "m_currentColor");

    refreshBaseQSS();

    m_barColorQss = QString("QScrollBar::handle:vertical {"
                                "background: rgba(%0, %1, %2, %3);"
                                "min-height: 25px;"
                            "}"
                            "QScrollBar::handle:horizontal {"
                                "background: rgba(%0, %1, %2, %3);"
                                "min-width: 25px;"
                            "}").arg(m_defaultColor.red()).arg(m_defaultColor.green()).arg(m_defaultColor.blue()).arg(m_defaultColor.alpha());

    setStyleSheet(m_baseQss + m_barColorQss);

    connect(this, &AnimatedHoverScrollBar::visibilityChanged, this, &AnimatedHoverScrollBar::updateMargin, Qt::DirectConnection);

}

void AnimatedHoverScrollBar::refreshBaseQSS(){
    m_baseQss = QString("QScrollBar:vertical {"
                        "border: %6px solid rgba(%7, %8, %9, %10);"
                        "background: rgb(50, 67, 87);"
                        "width: 15px;"
                        "margin: %0px 0px 0px 0px;"
                    "}"
                    "QScrollBar::add-line:vertical {"
                        "border: 0px solid grey;"
                        "background: rgba(%2, %3, %4, %5);"
                        "height: %0px;"
                        "subcontrol-position: top;"
                        "subcontrol-origin: margin;"
                    "}"
                    "QScrollBar::sub-line:vertical {"
                        "border: none;"
                        "background: rgba(%2, %3, %4, %5);"
                        "height: 0px;"
                        "subcontrol-position: bottom;"
                        "subcontrol-origin: margin;"
                    "}"
                     "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                        "background: none;"
                     "}"
                     "QScrollBar:horizontal {"
                        "border: %6px solid rgba(%7, %8, %9, %10);"
                        "background: rgba(%2, %3, %4, %5);"
                        "height: 15px;"
                        "margin: 0px px 0px %1px;"
                     "}"
                     "QScrollBar::add-line:horizontal {"
                         "border: none;"
                         "background: rgba(%2, %3, %4, %5);"
                         "width: %1px;"
                         "subcontrol-position: left;"
                         "subcontrol-origin: margin;"
                     "}"
                     "QScrollBar::sub-line:horizontal {"
                         "border: 0px solid grey;"
                         "background: none;"
                         "width: 0px;"
                         "subcontrol-position: right;"
                         "subcontrol-origin: margin;"
                     "}"
                      "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {"
                         "background: none;"
                     "}").arg(m_widthMargin).                   //%0
                          arg(m_heightMargin).                  //%1
                          arg(m_backgroundColor.red()).         //%2
                          arg(m_backgroundColor.green()).       //%3
                          arg(m_backgroundColor.blue()).        //%4
                          arg(m_backgroundColor.alpha()).       //%5
                          arg(m_borderWidth).                   //%6
                          arg(m_backgroundBorderColor.red()).   //%7
                          arg(m_backgroundBorderColor.green()). //%8
                          arg(m_backgroundBorderColor.blue()).  //%9
                          arg(m_backgroundBorderColor.alpha()); //%10

    setStyleSheet(m_baseQss + m_barColorQss);
}

void AnimatedHoverScrollBar::updateMargin(){
    if(m_parentTable){
        m_heightMargin = m_parentTable->verticalHeader()->isVisible() ? m_parentTable->verticalHeader()->width() : 0;
        m_widthMargin = m_parentTable->horizontalHeader()->isVisible() ? m_parentTable->horizontalHeader()->height() : 0;
        refreshBaseQSS();
    }
}

void AnimatedHoverScrollBar::setTransitionDuration(int duration){
    m_duration = duration;
    m_transition->setDuration(m_duration);
}

void AnimatedHoverScrollBar::setDefaultColor(const QColor& defaultColor){
    m_defaultColor = defaultColor;
    refreshBaseQSS();
}

void AnimatedHoverScrollBar::setHoverColor(const QColor& hoverColor){
    m_hoverColor = hoverColor;
    refreshBaseQSS();
}

void AnimatedHoverScrollBar::setPressedColor(const QColor& pressedColor){
    m_pressedColor = pressedColor;
    refreshBaseQSS();
}

void AnimatedHoverScrollBar::setBackgroundColor(const QColor& backgroundColor){
    m_backgroundColor = backgroundColor;
    refreshBaseQSS();
}

void AnimatedHoverScrollBar::setBackgroundBorderColor(const QColor& backgroundBorderColor){
    m_backgroundBorderColor = backgroundBorderColor;
    refreshBaseQSS();
}

void AnimatedHoverScrollBar::setBackgroundBorderRadius(unsigned int pixels){
    m_borderWidth = pixels;
    refreshBaseQSS();
}

bool AnimatedHoverScrollBar::event(QEvent *event){    
    switch(event->type()){
        case QEvent::HoverEnter:
            animateHover(true);
            break;
        case QEvent::HoverLeave:
            animateHover(false);
            break;
        case QEvent::MouseButtonPress:
            {
                QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
                if(mouseEvent->button() == Qt::LeftButton){
                    animateKeyPress(true);
                    m_dragging = true;
                    bool horizontal = this->orientation() == Qt::Horizontal;
                    QStyleOptionSlider opt;
                    initStyleOption(&opt);
                    QRect sr = style()->subControlRect(QStyle::CC_ScrollBar, &opt,
                                                       QStyle::SC_ScrollBarSlider, this);
                    m_pressedControl = style()->hitTestComplexControl(QStyle::CC_ScrollBar, &opt, mouseEvent->pos(), this);

                    QPoint click = mouseEvent->pos();
                    QPoint pressValue = click - sr.center() + sr.topLeft();
                    m_pressValue = horizontal ? pixelPosToRangeValue(pressValue.x()) : pixelPosToRangeValue(pressValue.y());
                    if (m_pressedControl == QStyle::SC_ScrollBarSlider) {
                        m_clickOffset = horizontal ? (click.x() - sr.x()) : (click.y() - sr.y());
                    }
                }
            }
            break;
        case QEvent::MouseButtonRelease:
            if(dynamic_cast<QMouseEvent*>(event)->button() == Qt::LeftButton){
                animateKeyPress(false);
                m_dragging = false;
            }
            break;
        case QEvent::MouseMove:

            if(m_dragging){
                QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
                bool horizontal = this->orientation() == Qt::Horizontal;
                QStyleOptionSlider opt;
                initStyleOption(&opt);

                if (!(mouseEvent->buttons() & Qt::LeftButton
                      ||  ((mouseEvent->buttons() & Qt::MidButton)
                           && style()->styleHint(QStyle::SH_ScrollBar_MiddleClickAbsolutePosition, &opt, this))))
                    return true;

                if (m_pressedControl == QStyle::SC_ScrollBarSlider) {
                    QPoint click = mouseEvent->pos();
                    int newPosition = pixelPosToRangeValue((horizontal ? click.x() : click.y()) - m_clickOffset);

                    int m = style()->pixelMetric(QStyle::PM_MaximumDragDistance, &opt, this);
                    if (m >= 0) {
                        QRect r = rect();
                        r.adjust(-m, -m, m, m);
                    }

                    setSliderPosition(newPosition);
                } else if (!style()->styleHint(QStyle::SH_ScrollBar_ScrollWhenPointerLeavesControl, &opt, this)) {

                    if (style()->styleHint(QStyle::SH_ScrollBar_RollBetweenButtons, &opt, this)
                            && m_pressedControl & (QStyle::SC_ScrollBarAddLine | QStyle::SC_ScrollBarSubLine)) {
                        QStyle::SubControl newSc = style()->hitTestComplexControl(QStyle::CC_ScrollBar, &opt, mouseEvent->pos(), this);
                        if (newSc == m_pressedControl)
                            return true; // nothing to do
                        if (newSc & (QStyle::SC_ScrollBarAddLine | QStyle::SC_ScrollBarSubLine)) {
                            QRect scRect = style()->subControlRect(QStyle::CC_ScrollBar, &opt, newSc, this);
                            scRect |= style()->subControlRect(QStyle::CC_ScrollBar, &opt, m_pressedControl, this);
                            m_pressedControl = newSc;
                            activateControl(m_pressedControl, 0);
                            update(scRect);
                            return true;
                        }
                    }
                }
                return true;
            }

            break;
        case QEvent::Show:
            emit visibilityChanged();
            break;
        case QEvent::Hide:
            emit visibilityChanged();
            break;
        default:
            break;
    }

    return QScrollBar::event(event);
}


void AnimatedHoverScrollBar::activateControl(uint control, int threshold){
    QAbstractSlider::SliderAction action = QAbstractSlider::SliderNoAction;
    switch (control) {
        case QStyle::SC_ScrollBarAddPage:
            action = QAbstractSlider::SliderPageStepAdd;
            break;
        case QStyle::SC_ScrollBarSubPage:
            action = QAbstractSlider::SliderPageStepSub;
            break;
        case QStyle::SC_ScrollBarAddLine:
            action = QAbstractSlider::SliderSingleStepAdd;
            break;
        case QStyle::SC_ScrollBarSubLine:
            action = QAbstractSlider::SliderSingleStepSub;
            break;
        case QStyle::SC_ScrollBarFirst:
            action = QAbstractSlider::SliderToMinimum;
            break;
        case QStyle::SC_ScrollBarLast:
            action = QAbstractSlider::SliderToMaximum;
            break;
        default:
            break;
    }

    if (action) {
        setRepeatAction(action, threshold);
        triggerAction(action);
    }
}

void AnimatedHoverScrollBar::setColor(const QColor& color){
    m_currentColor = color;

    m_barColorQss = QString("QScrollBar::handle:vertical {"
                                "background: rgb(%1, %2, %3);"
                                "min-height: 25px;"
                            "}"
                            "QScrollBar::handle:horizontal {"
                                "background: rgb(%1, %2, %3);"
                                "min-width: 25px;"
                            "}").arg(m_currentColor.red()).arg(m_currentColor.green()).arg(m_currentColor.blue());

    setStyleSheet(m_baseQss + m_barColorQss);
}

QColor AnimatedHoverScrollBar::currentColor(){
    return m_currentColor;
}

void AnimatedHoverScrollBar::animateHover(bool hoveringIn){
    if (m_transition->state() == QPropertyAnimation::State::Running) {
        m_transition->stop();
    }

    QColor startValue = m_currentColor;

    m_transition->setStartValue(startValue);
    m_transition->setEndValue(hoveringIn ? m_hoverColor : m_defaultColor);
    m_transition->setDuration(m_duration);

    m_transition->start();
}

void AnimatedHoverScrollBar::animateKeyPress(bool pressedIn){

    if (m_transition->state() == QPropertyAnimation::State::Running) {
        m_transition->stop();
    }

    QColor startValue = m_currentColor;

    m_transition->setStartValue(startValue);
    QColor endValue = pressedIn ? m_pressedColor : m_defaultColor; //Determine if the end should be pressed or default
    endValue = (this->underMouse() && !pressedIn) ? m_hoverColor : endValue;       //Determine if we release while still hovering over widget
    m_transition->setEndValue(endValue);
    m_transition->setDuration(m_duration);

    m_transition->start();
}

//Stripped from QScrollBar.cpp source and modified to override the horrid Windows OS dead zone for a scrollbaar drag
int AnimatedHoverScrollBar::pixelPosToRangeValue(int pos) const{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect gr = style()->subControlRect(QStyle::CC_ScrollBar, &opt,
                                       QStyle::SC_ScrollBarGroove, this);
    QRect sr = style()->subControlRect(QStyle::CC_ScrollBar, &opt,
                                       QStyle::SC_ScrollBarSlider, this);
    int sliderMin, sliderMax, sliderLength;

    if (orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
        if (layoutDirection() == Qt::RightToLeft)
            opt.upsideDown = !opt.upsideDown;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }

    return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin, sliderMax - sliderMin, opt.upsideDown);
}
