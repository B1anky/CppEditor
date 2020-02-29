#include "TreeView.h"

TreeView::TreeView(QWidget* parent) : QTreeView(parent){
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Base, QColor(20, 25, 31));
    palette.setBrush(QPalette::Text, QColor(208, 223, 230));

    setPalette(palette);
}
