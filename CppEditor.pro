#-------------------------------------------------
#
# Project created by QtCreator 2020-02-13T21:02:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CppEditor
TEMPLATE = app

CONFIG += c++14

SOURCES += \
        main.cpp \
        Highlighter.cpp \
        TextEditor.cpp \
    CppIDEWidget.cpp \
    CppEditorToolBar.cpp \
    TreeView.cpp \
    TextEditorLineNumberArea.cpp \
    AnimatedHoverScrollBar.cpp \
    TextOverviewTextEdit.cpp


HEADERS += \
        ConvenienceFunctions.h \
        Highlighter.h \
        TextEditor.h \
    CppIDEWidget.h \
    CppEditorToolBar.h \
    TreeView.h \
    TextEditorLineNumberArea.h \
    AnimatedHoverScrollBar.h \
    TextOverviewTextEdit.h


