#ifndef CPPIDEWIDGET_H
#define CPPIDEWIDGET_H

#include <QMainWindow>
#include <QMap>

//Forward declaring is more efficient for compiling
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QComboBox;
class QLabel;
class QSplitter;
class CppEditorToolBar;
class TreeView;
class TextEditor;


class CppIDEWidget : public QMainWindow{

    Q_OBJECT

public:

    CppIDEWidget(QWidget* parent = nullptr);

    ~CppIDEWidget();

private:

    void setupCentralWidget();

private:
    CppEditorToolBar* m_runBuildToolBar;
    TreeView*         m_treeView;
    TextEditor*       m_editor;
    QWidget*          m_centralWidget;
    QSplitter*        m_splitter;
    QVBoxLayout*      m_centralWidgetVLayout;
    QHBoxLayout*      m_centralWidgetHLayout;

};

#endif // CPPIDEWIDGET_H
