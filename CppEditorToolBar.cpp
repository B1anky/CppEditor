#include "CppEditorToolBar.h"

#include <QPushButton>
#include <QMap>

CppEditorToolBar::CppEditorToolBar(QWidget* parent) :
    QToolBar(parent),
    m_runButton(nullptr),
    m_buildButton(nullptr)
{
    QSize buttonSize(64, 64);
    QSize iconSize(50, 50);
    QString buttonStyle = "QPushButton{"
                          "    border: none;"
                          "    background-color: none;"
                          "}"
                          "QPushButton:hover:!pressed{"
                          "    background-color: rgb(125, 125, 125)"
                          "}"
                          "QPushButton:hover:pressed{"
                          "    background-color: rgb(85, 85, 85)"
                          "}";

    m_runButton = new QPushButton;
    m_runButton->setIcon(QIcon("assets/run.png"));
    m_runButton->setIconSize(iconSize);
    m_runButton->setFixedSize(buttonSize);
    m_runButton->setToolTip("Run Ctrl + r");
    m_runButton->setStyleSheet(buttonStyle);

    m_buildButton = new QPushButton;
    m_buildButton->setIcon(QIcon("assets/build.png"));
    m_buildButton->setIconSize(iconSize);
    m_buildButton->setFixedSize(buttonSize);
    m_buildButton->setToolTip("Build Ctrl + b");
    m_buildButton->setStyleSheet(buttonStyle);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addWidget(spacer);

    m_actionMap[addWidget(m_runButton)]   = m_runButton;
    m_actionMap[addWidget(m_buildButton)] = m_buildButton;

    setStyleSheet("QToolBar {"
                  "    background: rgb(20, 25, 31);"
                  "}");

}
