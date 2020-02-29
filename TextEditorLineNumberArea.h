#ifndef TEXTEDITORLINENUMBERAREA_H
#define TEXTEDITORLINENUMBERAREA_H

#include <QWidget>
class QPaintEvent;
class QResizeEvent;
class QSize;

class TextEditor;

class TextEditorLineNumberArea : public QWidget{

    Q_OBJECT

public:

    TextEditorLineNumberArea(TextEditor* editor = nullptr);

    QSize sizeHint() const override;

protected:

    void paintEvent(QPaintEvent *event) override;

private:

    TextEditor* m_textEditor;

};
#endif // TEXTEDITORLINENUMBERAREA_H
