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

#ifndef ANIMATEDHOVERBUTTON_H
#define ANIMATEDHOVERBUTTON_H

#include <QScrollBar>
#include <QPropertyAnimation>
#include <QTableWidget>
#include <QHeaderView>
#include <QEvent>

class AnimatedHoverScrollBar : public QScrollBar{

    Q_OBJECT

    Q_PROPERTY(QColor m_currentColor READ currentColor WRITE setColor)

public:

    AnimatedHoverScrollBar(const QColor& defaultColor    = QColor(90, 94, 98), const QColor& hoverColor = QColor(18, 100, 145), const QColor& pressedColor = QColor(38, 127, 181),
                           const QColor& backgroundColor = QColor(50, 67, 87), const QColor& backgroundBorderColor = QColor(38, 43, 48),
                           unsigned int borderWidth = 2, QWidget* parent = nullptr);

    void setTransitionDuration(int duration);

    void setDefaultColor(const QColor& defaultColor);

    void setHoverColor(const QColor& hoverColor);

    void setPressedColor(const QColor& pressedColor);

    void setBackgroundColor(const QColor& defaultColor);

    void setBackgroundBorderColor(const QColor& color);

    void setBackgroundBorderRadius(unsigned int pixels);

protected:

    bool event(QEvent* event) override;

    void activateControl(uint control, int threshold);

private:

    void setColor(const QColor& color);

    QColor currentColor();

    void animateHover(bool hoveringIn);

    void animateKeyPress(bool pressedIn);

    void refreshBaseQSS();

    int pixelPosToRangeValue(int pos) const;

private slots:

    void updateMargin();

signals:

    void visibilityChanged();

private:

    QPropertyAnimation* m_transition;
    int                 m_duration;
    int                 m_heightMargin;
    int                 m_widthMargin;
    int                 m_borderWidth;
    QColor              m_defaultColor;
    QColor              m_hoverColor;
    QColor              m_pressedColor;
    QColor              m_backgroundColor;
    QColor              m_backgroundBorderColor;
    QColor              m_currentColor;
    QString             m_baseQss;
    QString             m_barColorQss;
    QTableWidget*       m_parentTable;
    bool                m_dragging;
    int                 m_clickOffset;
    int                 m_snapBackPosition;
    int                 m_pressValue;
    QStyle::SubControl  m_pressedControl;

};

#endif // ANIMATEDHOVERBUTTON_H
