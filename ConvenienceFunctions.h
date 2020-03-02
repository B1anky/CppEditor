#ifndef CONVENIENCEFUNCTIONS_H
#define CONVENIENCEFUNCTIONS_H

#include <QGuiApplication>
#include <QWidget>
#include <QScreen>
#include <QWindow>
#include <QDebug>

//This will center a widget on the current screen/monitor that the cursor is on,
//in case the user moves the widget from one monitor to another
//It also has an optional screenNum parameter that can force which monitor to center on
//Note: If the screenNum provided is invalid, it will deem the default behavior of -1
static inline void centerWidget(QWidget* widget, int screenNum = -1){

    if(widget == nullptr) return;

    //If the widget hasn't been shown yet, the layout is dirty and may not have resize its contents appropriately yet
    //This will force is before being shown to obtain the correct sizes
    if(!widget->isVisible() && widget->layout() != nullptr){
        widget->layout()->invalidate();
        widget->layout()->activate();
    }

    QScreen* mouseScreen(nullptr);

    if(screenNum == -1 || screenNum >= QGuiApplication::screens().size()){
        mouseScreen = QGuiApplication::screenAt(QCursor::pos());
    }else{
        mouseScreen = QGuiApplication::screens().at(screenNum);
    }

    QRect mouseScreenGeometry = mouseScreen->geometry();

    int xPos = mouseScreenGeometry.x() + (mouseScreenGeometry.width() / 2) - (widget->width() / 2);
    int yPos = (mouseScreenGeometry.height() / 2) - (widget->height() / 2);

    widget->move(xPos, yPos);

}

#endif // CONVENIENCEFUNCTIONS_H
