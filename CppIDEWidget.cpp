#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QFontDialog>
#include <QSplitter>

#include "CppIDEWidget.h"
#include "CppEditorToolBar.h"
#include "ConvenienceFunctions.h"
#include "TextEditor.h"
#include "TreeView.h"

CppIDEWidget::CppIDEWidget(QWidget *parent)
    : QMainWindow(parent),
      m_runBuildToolBar(nullptr),
      m_treeView(nullptr),
      m_centralWidget(nullptr),
      m_splitter(nullptr),
      m_centralWidgetVLayout(nullptr),
      m_centralWidgetHLayout(nullptr)

{

    setObjectName("CppIdeWidget");
    setAutoFillBackground(true);
    setStyleSheet("CppIDEWidget {"
                  "    background: rgb(20, 25, 31);"
                  "    width:   5px;" /* when vertical */
                  "    height:  5px;" /* when horizontal */
                  "}");

    this->resize(1000, 750);
    setupCentralWidget();

    centerWidget(this);

}

CppIDEWidget::~CppIDEWidget(){

}


void CppIDEWidget::setupCentralWidget(){

    m_runBuildToolBar = new CppEditorToolBar;
    addToolBar(Qt::ToolBarArea::LeftToolBarArea, m_runBuildToolBar);

    m_centralWidget = new QWidget;
    setCentralWidget(m_centralWidget);
    m_centralWidgetHLayout = new QHBoxLayout;
    m_centralWidget->setLayout(m_centralWidgetHLayout);

    m_centralWidget->setObjectName("CppIDEWidget");
    m_centralWidget->setStyleSheet("#CppIDEWidget{"
                                   "    background-color: solid rgba(0,0,0,0);"
                                   "}");

    m_treeView = new TreeView;
    m_splitter = new QSplitter;
    m_editor   = new TextEditor(true);

    m_centralWidgetHLayout->addWidget(m_splitter);    
    m_centralWidgetHLayout->setContentsMargins(0,0,0,0);

    m_splitter->setStyleSheet("QSplitter::handle{ "
                              "    background : rgba(0,0,0,255); "
                              "}");

    m_splitter->addWidget(m_treeView);
    m_splitter->addWidget(m_editor);

    m_splitter->setStretchFactor(0, 1);
    m_splitter->setStretchFactor(1, 5);

}
