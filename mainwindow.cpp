#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    QMetaObject::connectSlotsByName(this);
    this->resize(854, 480);
    this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));

    mulVidWidget = new MultiVideoViewer(this);
    mulVidWidget->setFixedSize(this->size());
    mulVidWidget->setVideoSize(200, 200, 200, 200);

    sidebar = new Side_bar(this);
    sidebar->setFixedSize(300, 480);
}

MultiVideoViewer *MainWindow::getMultiVideoViewerWidget()
{
    return mulVidWidget;
}

MainWindow::~MainWindow()
{

}
