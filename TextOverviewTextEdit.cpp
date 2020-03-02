#include "TextOverviewTextEdit.h"

#include <QVBoxLayout>
#include "TextEditor.h"

TextOverviewTextEdit::TextOverviewTextEdit(QWidget* parent) : QPlainTextEdit(parent){
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setReadOnly(true);
    setFrameStyle(QFrame::NoFrame);
    setLineWrapMode(QPlainTextEdit::NoWrap);
    if(qobject_cast<TextEditor*>(parentWidget())){
        setFocusProxy(parentWidget());
    }


}

//This widget will not scroll on its own, its companion text editor will do that
void TextOverviewTextEdit::wheelEvent(QWheelEvent* event){
    if(qobject_cast<TextEditor*>(parentWidget())){
        qobject_cast<TextEditor*>(parentWidget())->wheelEvent(event);
    }else{
        event->accept();
    }
}

void TextOverviewTextEdit::keyPressEvent(QKeyEvent* event){
    if(qobject_cast<TextEditor*>(parentWidget())){
        qobject_cast<TextEditor*>(parentWidget())->keyPressEvent(event);
    }else{
        event->accept();
    }
}

void TextOverviewTextEdit::keyReleaseEvent(QKeyEvent* event){
    if(qobject_cast<TextEditor*>(parentWidget())){
        qobject_cast<TextEditor*>(parentWidget())->keyReleaseEvent(event);
    }else{
        event->accept();
    }
}

void TextOverviewTextEdit::mousePressEvent(QMouseEvent* event){
    if(qobject_cast<TextEditor*>(parentWidget())){
        qobject_cast<TextEditor*>(parentWidget())->mousePressEvent(event);
    }else{
        event->accept();
    }
}

void TextOverviewTextEdit::mouseReleaseEvent(QMouseEvent* event){
    if(qobject_cast<TextEditor*>(parentWidget())){
        qobject_cast<TextEditor*>(parentWidget())->mouseReleaseEvent(event);
    }else{
        event->accept();
    }
}

void TextOverviewTextEdit::mouseMoveEvent(QMouseEvent* event){
    if(qobject_cast<TextEditor*>(parentWidget())){
        qobject_cast<TextEditor*>(parentWidget())->mouseMoveEvent(event);
    }else{
        event->accept();
    }
}
