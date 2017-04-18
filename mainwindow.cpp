#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    QMetaObject::connectSlotsByName(this);
    this->resize(mainWindowWidth, mainWindowHeight);
    this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));

    grid_main_layout = new QGridLayout;
    cen = new QWidget(this);
    this->setCentralWidget(cen);
    cen->setLayout(grid_main_layout);

    sidebar = new Side_bar(cen);
    sidebar->setFixedSize(sideBarWidth, mainWindowHeight);
    grid_main_layout->addWidget(sidebar, 0, 0);

    mulVidWidget = new MultiVideoViewer(cen);
    mulVidWidget->setFixedSize(multiVideoWidth, mainWindowHeight);
    mulVidWidget->setVideoSize(200, 200, 200, 200);
    grid_main_layout->addWidget(mulVidWidget, 0, 1);

    fsVidWidget = new fullScreenVideoViewer(cen);
    fsVidWidget->setFixedSize(mainWindowWidth, mainWindowHeight);
    fsVidWidget->setVideoSize(fsVidWidget->size());

    sidebar->setVisible(true);
    mulVidWidget->setVisible(true);
    fsVidWidget->setVisible(false);
}

MultiVideoViewer *MainWindow::getMultiVideoViewerWidget()
{
    return mulVidWidget;
}

fullScreenVideoViewer *MainWindow::getFullScreenVideoViewerWidget()
{
    return fsVidWidget;
}

MainWindow::~MainWindow()
{

}
