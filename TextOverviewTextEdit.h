#ifndef TEXTOVERVIEWTEXTEDIT_H
#define TEXTOVERVIEWTEXTEDIT_H

#include <QPlainTextEdit>

class TextOverviewTextEdit : public QPlainTextEdit{

    Q_OBJECT

    friend class TextEditor;

public:

    TextOverviewTextEdit(QWidget* parent = nullptr);

protected:

    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

};

#endif // TEXTOVERVIEWTEXTEDIT_H
