#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    QMetaObject::connectSlotsByName(this);
    this->resize(854, 480);
    this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
    sidebar = new Side_bar(this);
    sidebar->setFixedSize(300, 480);
}

MainWindow::~MainWindow()
{

}
