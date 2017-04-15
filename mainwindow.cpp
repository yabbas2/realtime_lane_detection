#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMetaObject::connectSlotsByName(this);
    this->resize(400, 300);
    this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
}

MainWindow::~MainWindow()
{

}
