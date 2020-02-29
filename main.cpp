#include "CppIDEWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CppIDEWidget w;
    w.show();

    return a.exec();
}
