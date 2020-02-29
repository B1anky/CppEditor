#include "TextEditorLineNumberArea.h"

#include "TextEditor.h"
#include <QPainter>
#include <QTextBlock>

TextEditorLineNumberArea::TextEditorLineNumberArea(TextEditor* editor) : QWidget(editor), m_textEditor(editor){
    /* NOP */
}


void TextEditorLineNumberArea::paintEvent(QPaintEvent *event){
    m_textEditor->lineNumberAreaPaintEvent(event);
}

QSize TextEditorLineNumberArea::sizeHint() const{
    return QSize(m_textEditor->lineNumberAreaWidth(), 0);
}
