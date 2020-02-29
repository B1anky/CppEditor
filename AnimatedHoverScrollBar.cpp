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

AnimatedHoverScrollBar::AnimatedHoverScrollBar(QColor startColor, QColor endColor, QWidget *parent) :
    QScrollBar(parent),
    m_transition(nullptr),
    m_duration(1000),
    m_heightMargin(0), m_widthMargin(0),
    m_startColor(startColor),
    m_endColor(endColor),
    m_parentTable(qobject_cast<QTableWidget*>(parent))
{

    m_transition = new QPropertyAnimation(this, "m_currentColor");

    refreshBaseQSS();

    m_barColorQss = QString("QScrollBar::handle:vertical {"
                                "background: rgb(%1, %2, %3);"
                                "min-height: 25px;"
                            "}"
                            "QScrollBar::handle:horizontal {"
                                "background: rgb(%1, %2, %3);"
                                "min-width: 25px;"
                            "}").arg(m_startColor.red()).arg(m_startColor.green()).arg(m_startColor.blue());

    setStyleSheet(m_baseQss + m_barColorQss);

    connect(this, &AnimatedHoverScrollBar::visibilityChanged, this, &AnimatedHoverScrollBar::updateMargin, Qt::DirectConnection);
}

void AnimatedHoverScrollBar::refreshBaseQSS(){
    m_baseQss = QString("QScrollBar:vertical {"
                        "border: 2px solid grey;"
                        "background: rgb(50, 67, 87);"
                        "width: 15px;"
                        "margin: %1px 0px 0px 0px;"
                    "}"
                    "QScrollBar::add-line:vertical {"
                        "border: 0px solid grey;"
                        "background: rgb(50, 67, 87);"
                        "height: %1px;"
                        "subcontrol-position: top;"
                        "subcontrol-origin: margin;"
                    "}"
                    "QScrollBar::sub-line:vertical {"
                        "border: 0px solid grey;"
                        "background: rgb(50, 67, 87);"
                        "height: 0px;"
                        "subcontrol-position: bottom;"
                        "subcontrol-origin: margin;"
                    "}"
                     "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                        "background: none;"
                     "}"
                     "QScrollBar:horizontal {"
                        "border: 2px solid grey;"
                        "background: rgb(50, 67, 87);"
                        "height: 15px;"
                        "margin: 0px px 0px %2px;"
                     "}"
                     "QScrollBar::add-line:horizontal {"
                         "border: 0px solid grey;"
                         "background: rgb(50, 67, 87);"
                         "width: %2px;"
                         "subcontrol-position: left;"
                         "subcontrol-origin: margin;"
                     "}"
                     "QScrollBar::sub-line:horizontal {"
                         "border: 0px solid grey;"
                         "background: white;"
                         "width: 0px;"
                         "subcontrol-position: right;"
                         "subcontrol-origin: margin;"
                     "}"
                      "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {"
                         "background: none;"
                     "}").arg(m_widthMargin).arg(m_heightMargin);

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

bool AnimatedHoverScrollBar::event(QEvent *event){
    switch(event->type()){
        case QEvent::HoverEnter:
            animateHover(true);
            break;
        case QEvent::HoverLeave:
            animateHover(false);
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

void AnimatedHoverScrollBar::setColor(QColor color){
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
    QColor startValue = hoveringIn ? m_startColor : m_endColor;

    if (m_transition->state() == QPropertyAnimation::State::Running) {
        startValue = m_currentColor;
        m_transition->stop();
    }

    m_transition->setStartValue(startValue);
    m_transition->setEndValue(hoveringIn ? m_endColor : m_startColor);
    m_transition->setDuration(m_duration);

    m_transition->start();
}
