#ifndef CPPEDITORTOOLBAR_H
#define CPPEDITORTOOLBAR_H

#include <QToolBar>

class QPushButton;
class QAction;

class CppEditorToolBar : public QToolBar{

    friend class CppIDEWidget;

    Q_OBJECT
public:
    CppEditorToolBar(QWidget* parent = nullptr);

private:
    QPushButton* m_runButton;
    QPushButton* m_buildButton;

    QMap<QAction*, QPushButton*> m_actionMap;

};

#endif // CPPEDITORTOOLBAR_H
