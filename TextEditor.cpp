#include <QTextEdit>
#include <QPalette>
#include <QKeyEvent>
#include <QApplication>
#include <QTextDocumentFragment>
#include <QPainter>
#include <QTextBlock>


#include <QDebug>

#include "TextEditor.h"
#include "TextEditorLineNumberArea.h"
#include "Highlighter.h"
#include "AnimatedHoverScrollBar.h"

TextEditor::TextEditor(bool tabsAsSpaces, QWidget* parent) :
    QPlainTextEdit(parent),
    m_tabsAsSpaces(tabsAsSpaces),
    m_highlighter(nullptr),
    m_horizontalScrollBar(nullptr),
    m_verticalScrollBar(nullptr)
{
    m_highlighter = new Highlighter(this);
    m_highlighter->setDocument(this->document());
    m_font.setFamily("Consolas");
    m_font.setPointSize(12);

    setFrameStyle(QFrame::NoFrame);

    //setAcceptRichText(false);

    m_horizontalScrollBar = new AnimatedHoverScrollBar(QColor(125, 125, 95), QColor(50, 88, 187));
    m_horizontalScrollBar->setTransitionDuration(500);
    setHorizontalScrollBar(m_horizontalScrollBar);

    m_verticalScrollBar = new AnimatedHoverScrollBar(QColor(125, 125, 95), QColor(50, 88, 187));
    m_horizontalScrollBar->setTransitionDuration(500);
    setVerticalScrollBar(m_verticalScrollBar);

    setFont(m_font);
    //setTab
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Base, QColor(20, 25, 31));
    palette.setBrush(QPalette::Text, QColor(208, 223, 230));

    setPalette(palette);

    m_lineNumberArea = new TextEditorLineNumberArea(this);


    connect(this, &TextEditor::blockCountChanged,     this, &TextEditor::updateLineNumberAreaWidth);
    connect(this, &TextEditor::updateRequest,         this, &TextEditor::updateLineNumberArea);
    connect(this, &TextEditor::cursorPositionChanged, this, &TextEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

}

int TextEditor::lineNumberAreaWidth(){

    int digits = 1;
    int max = qMax(1, this->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 15 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void TextEditor::updateLineNumberAreaWidth(int /* newBlockCount */){
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void TextEditor::updateLineNumberArea(const QRect &rect, int dy){

    if (dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);      
}

void TextEditor::resizeEvent(QResizeEvent *e){
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void TextEditor::highlightCurrentLine(){
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(255, 255, 255, 25);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void TextEditor::lineNumberAreaPaintEvent(QPaintEvent *event){
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), QColor(50, 67, 87));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
          QString number = QString::number(blockNumber + 1);
          painter.setPen(QColor(225, 225, 225));
          painter.setFont(this->font());
          painter.drawText(0, top, m_lineNumberArea->width(), fontMetrics().height(),
                           Qt::AlignCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void TextEditor::keyPressEvent(QKeyEvent* keyEvent){

    if(keyEvent->key() == Qt::Key_Tab && m_tabsAsSpaces && textCursor().selectedText().size() == 0){
        insertPlainText("    ");
        keyEvent->accept();
        return;
    }else if(keyEvent->key() == Qt::Key_Tab && !(QApplication::keyboardModifiers() & Qt::ShiftModifier) && textCursor().selection().toPlainText().contains("\n")){
        textCursor().beginEditBlock();
        indent();
        keyEvent->accept();
        textCursor().endEditBlock();
        return;
    }else if(keyEvent->key() == Qt::Key_Backtab){
        textCursor().beginEditBlock();
        unindent();
        keyEvent->accept();
        textCursor().endEditBlock();
        return;
    }

    QPlainTextEdit::keyPressEvent(keyEvent);

}

//multi-line indentation
void TextEditor::indent(){
    QTextCursor cur     = this->textCursor();
    QString insertChars = m_tabsAsSpaces ? "    " : "\t";
    int offset          = m_tabsAsSpaces ? 4 : 1;
    int offsetMult      = 0;
    int origPos         = cur.position();
    int origAnchor      = cur.anchor();
    int origStart       = cur.selectionStart();
    int origEnd         = cur.selectionEnd();

    int lineSpan = cur.selection().toPlainText().count("\n");
    if(lineSpan > 0) ++lineSpan; //This is necessary since if at least one newline is found, it's spanning two lines

    //We iterate top-down from the first selected line until we reach lineSpan lines down
    //and have inserted either tabs or white spaces to the front of each start of line
    cur.setPosition(cur.selectionStart(), QTextCursor::MoveAnchor);

    indentHelper(cur, insertChars, lineSpan, offsetMult);

    while(lineSpan > 0){
        cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
        indentHelper(cur, insertChars, lineSpan, offsetMult);
    }

    //This will keep the original selection after tabbing so it can be done multiple times sequentially
    if(origPos > origAnchor){ //-> leftMouseClick + moved down or shift + key_down
        cur.setPosition(origStart + offset, QTextCursor::MoveAnchor);
        cur.setPosition(origEnd   + (offset * offsetMult), QTextCursor::KeepAnchor);
    }else{                    //-> leftMouseClick + moved up or shift + key_up
        cur.setPosition(origEnd   + (offset * offsetMult), QTextCursor::MoveAnchor);
        cur.setPosition(origStart + offset, QTextCursor::KeepAnchor);
    }

    this->setTextCursor(cur);

}

void TextEditor::indentHelper(QTextCursor& cur, const QString& insertChars, int& lineSpan, int& offsetMult){
    cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    cur.insertText(insertChars);
    --lineSpan;
    ++offsetMult;
}

//multi-line unindentation
void TextEditor::unindent(){
    QTextCursor cur  = this->textCursor();
    QString origText = cur.selection().toPlainText();
    int origPos      = cur.position();
    int origAnchor   = cur.anchor();
    int origStart    = cur.selectionStart();
    int origEnd      = cur.selectionEnd();

    //This is used to fix up highlighting and know how many characters, per line, from top to bottom, were removed from each
    //Much like the int offsetMult from indent, but the issue here is that some things may or may not be untabbed,
    //but while tabbing it's gauranteed the same amount of characters per line
    QVector<int> offsetAccumVec;

    //This is standard behavior for highlighting text on a single line and doing a shift-tab, it's more like a delete selection
    if(!origText.contains("\n") && origText.length() > 0){
        cur.removeSelectedText();
        return;
    }

    //Top down line-by-line approach
    cur.setPosition(cur.selectionStart());
    cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    cur.movePosition(QTextCursor::EndOfLine,   QTextCursor::KeepAnchor);
    QString curLineText = cur.selectedText();
    cur.clearSelection();

    //Implies single-line unindent, caret is before words on a line and user has no selection highlighted
    if(origStart == origEnd && origPos > 0){
        cur.setPosition(origPos, QTextCursor::MoveAnchor);
        unindentHelper(cur, curLineText, offsetAccumVec, 0, origPos);
    }
    //Implies Multi-line unindent, at least a single newline character is highlighted, start and end don't matter
    else{

        int lineSpan = origText.count("\n");
        if(lineSpan > 0) ++lineSpan; //This is necessary since if at least one newline is found, it's spanning two lines

        int origLineSpan = lineSpan;

        unindentHelper(cur, curLineText, offsetAccumVec, lineSpan);
        --lineSpan;

        while(lineSpan > 0){
            cur.movePosition(QTextCursor::Down,        QTextCursor::MoveAnchor);
            cur.movePosition(QTextCursor::EndOfLine,   QTextCursor::MoveAnchor);
            cur.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
            curLineText = cur.selectedText();
            cur.clearSelection();

            unindentHelper(cur, curLineText, offsetAccumVec, lineSpan);
            --lineSpan;

        }

        //This will keep the original selection after tabbing so it can be done multiple times sequentially
        if(origPos > origAnchor){ //-> leftMouseClick + moved down or shift + key_down

            cur.setPosition(origStart - offsetAccumVec.first(), QTextCursor::MoveAnchor);
            //This will drag down all of the way to the last selected line
            cur.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, origLineSpan);
            cur.setPosition(origEnd - std::accumulate(offsetAccumVec.begin(), offsetAccumVec.end(), 0), QTextCursor::KeepAnchor);

        }else{                    //-> leftMouseClick + moved up or shift + key_up

            cur.setPosition(origEnd - std::accumulate(offsetAccumVec.begin(), offsetAccumVec.end(), 0), QTextCursor::MoveAnchor);
            //This will drag up all of the way to the first selected line
            cur.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor, origLineSpan);
            cur.setPosition(origStart - offsetAccumVec.first(), QTextCursor::KeepAnchor);

        }

        this->setTextCursor(cur);

    }
}

//If origPos == 0, this implies the origPos should be at the front of the first non-whitespace character
void TextEditor::unindentHelper(QTextCursor& cur, const QString& curLineString, QVector<int>& offsetAccumVec, int lineSpan, int origPos){

    QTextCursor isAtFrontOfLineCur = cur;
    int currentCaretPos = isAtFrontOfLineCur.position();
    isAtFrontOfLineCur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    int lineStartCaretPos = isAtFrontOfLineCur.position();

    //Finds first non-whitespace character on a line
    unsigned int wordStart = curLineString.toStdString().find_first_not_of(' ');

    //If we're on a line that's empty, save for maybe a "\n" we want to ignore the untab operation
    if((lineSpan == 0 && currentCaretPos == lineStartCaretPos) || wordStart == 0){
        offsetAccumVec.push_back(0);
        return;
    }

    //This sets the caret to the correct position
    //This will be 0 on the highlighted case, in which case we want something else
    cur.setPosition(origPos, QTextCursor::MoveAnchor);

    //If we're on the 0th character on the line, you can't untab anything
    //if the line contains no text other than a newline
    if(wordStart == std::string::npos){
        wordStart = curLineString.length();
    }

    //i.e. "____|someText" OR "\t|someText", "\t__|someText" only tabs or whitespace chars
    QString     textBeforeCaret       = curLineString.mid(0, wordStart);

    std::string textBeforeCaretStdStr = textBeforeCaret.toStdString();

    bool whiteSpacesOnly = std::all_of(textBeforeCaretStdStr.begin(), textBeforeCaretStdStr.end(), isspace);

    if(whiteSpacesOnly){

        //Auto-unindenting will attempt to unindent to the next modulo tab value (i.e. "\t__|someText" case would only remove two spaces)
        int unindentAmount = 1; //assume tabs, by default

        //In case the user only has whitespaces before thier caret, we want to remove more than just a single tab character
        if(textBeforeCaretStdStr.find_first_not_of(' ') == std::string::npos){
            unindentAmount = (textBeforeCaret.length() % 4 == 0) ? 4 : textBeforeCaret.length() % 4;
        }

        //If this spans multiple lines, then we need to set the caret to the front of the first word
        if(lineSpan > 0){
            cur.setPosition(lineStartCaretPos + wordStart, QTextCursor::MoveAnchor);
        }

        int offset = (lineStartCaretPos + wordStart) - unindentAmount;
        //Ensures bound-safety (setPosition doesn't like being negative, although it won't break anything)
        if(offset < lineStartCaretPos){
            offset = lineStartCaretPos;
        }

        cur.setPosition(offset, QTextCursor::KeepAnchor);

        //Do it one batch operation so undo is happier and treats it in the history as a single operation
        cur.removeSelectedText();

        //This keeps track, from top to bottom, how many characters were removed
        offsetAccumVec.push_back((lineStartCaretPos + wordStart) - offset);

    }

}
