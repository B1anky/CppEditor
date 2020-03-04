#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QFont>

//Forward declarations are faster for compilation
class Highlighter;
class TextEditorLineNumberArea;
class TextOverviewTextEdit;
class AnimatedHoverScrollBar;

class QVBoxLayout;
class QKeyEvent;
class QPaintEvent;
class QResizeEvent;
class QSize;

class TextEditor : public QPlainTextEdit{

    Q_OBJECT

    friend class TextOverviewTextEdit;

public:

    TextEditor(bool tabsAsSpaces = false, QWidget* parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:

    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent* keyEvent) override;
    void keyReleaseEvent(QKeyEvent* keyEvent) override;

    void indent();
    void indentHelper(QTextCursor& cursor, const QString& insertChars, int& lineSpan, int& offsetMult);
    void unindent();
    void unindentHelper(QTextCursor& cur, const QString& curLineString, QVector<int>& offsetAccumVec, int lineSpan, int origPos = 0);


protected slots:
    void onTextChanged();
    void scrollOverview(int scrollValue);
    void highlightInOverviewtextEdit();
private slots:

    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    TextEditorLineNumberArea* m_lineNumberArea;
    Highlighter*              m_highlighter;
    QFont                     m_font;
    bool                      m_tabsAsSpaces;
    QTextCursor               m_selectBlockAnchor;
    AnimatedHoverScrollBar*   m_horizontalScrollBar;
    AnimatedHoverScrollBar*   m_verticalScrollBar;
    TextOverviewTextEdit*     m_textOverviewTextEdit;
    int                       m_textOverviewTextEditMaxWidth;
    Highlighter*              m_textOverviewHighlighter;
    QVBoxLayout*              m_textOverviewTextEditLayout;

};

#endif // TEXTEDITOR_H
