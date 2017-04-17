#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    QMetaObject::connectSlotsByName(this);
    this->resize(854, 480);
    this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));

    grid_main_layout = new QGridLayout;
    cen = new QWidget(this);
    this->setCentralWidget(cen);
    cen->setLayout(grid_main_layout);

    sidebar = new Side_bar(cen);
    sidebar->setFixedSize(300, 480);
    grid_main_layout->addWidget(sidebar, 0, 0);

    mulVidWidget = new MultiVideoViewer(cen);
    mulVidWidget->setFixedSize(854-300, 480);
    mulVidWidget->setVideoSize(200, 200, 200, 200);
    grid_main_layout->addWidget(mulVidWidget, 0, 1);
}

MultiVideoViewer *MainWindow::getMultiVideoViewerWidget()
{
    return mulVidWidget;
}

MainWindow::~MainWindow()
{

}
