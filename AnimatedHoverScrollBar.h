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
    AnimatedHoverScrollBar(QColor startColor, QColor endColor, QWidget*  parent = nullptr);

    void setTransitionDuration(int duration);

    bool event(QEvent *event);

    void setColor(QColor color);

    QColor currentColor();

private slots:
    void updateMargin();

signals:
    void visibilityChanged();

private:
    void animateHover(bool hoveringIn);
    void refreshBaseQSS();

    QPropertyAnimation* m_transition;
    int                 m_duration;
    int                 m_heightMargin;
    int                 m_widthMargin;
    QColor              m_startColor;
    QColor              m_endColor;
    QColor              m_currentColor;
    QString             m_baseQss;
    QString             m_barColorQss;
    QTableWidget*       m_parentTable;
};

#endif // ANIMATEDHOVERBUTTON_H
