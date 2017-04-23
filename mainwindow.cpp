#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    QMetaObject::connectSlotsByName(this);
    this->setFixedSize(mainWindowWidth, mainWindowHeight);
    this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));

    grid_main_layout = new QGridLayout;
    cen1 = new QWidget(this);
    cen2 = new QWidget(this);
    cen1->setFixedSize(this->size());
    cen2->setFixedSize(this->size());

    cen1->setLayout(grid_main_layout);

    sidebar = new Side_bar(cen1);
    sidebar->setFixedSize(sideBarWidth, mainWindowHeight);
    grid_main_layout->addWidget(sidebar, 0, 0);

    mulVidWidget = new MultiVideoViewer(cen1);
    mulVidWidget->setFixedSize(multiVideoWidth, mainWindowHeight);
    mulVidWidget->setVideoSize(200, 200, 200, 200);
    grid_main_layout->addWidget(mulVidWidget, 0, 1);

    fsVidWidget = new fullScreenVideoViewer(cen2);
    fsVidWidget->setFixedSize(mainWindowWidth, mainWindowHeight);
    fsVidWidget->setVideoSize(fsVidWidget->size());

    vidWid = new VideoWidget(cen2);
    vidWid->setFixedSize(videoWidgetWidth, mainWindowHeight);

    cen1->setVisible(true);
    cen2->setVisible(false);

    connect(mulVidWidget->getVideoWidget(0), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
    connect(mulVidWidget->getVideoWidget(1), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
    connect(mulVidWidget->getVideoWidget(2), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
    connect(mulVidWidget->getVideoWidget(3), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
    connect(vidWid, SIGNAL(switchToMain()), this, SLOT(switchToMain()));
}

MultiVideoViewer *MainWindow::getMultiVideoViewerWidget()
{
    return mulVidWidget;
}

fullScreenVideoViewer *MainWindow::getFullScreenVideoViewerWidget()
{
    return fsVidWidget;
}

VideoWidget *MainWindow::getVideoWidget()
{
    return vidWid;
}

void MainWindow::switchToFullScreen()
{
    cen1->setVisible(false);
    cen2->setVisible(true);
}

void MainWindow::switchToMain()
{
    cen1->setVisible(true);
    cen2->setVisible(false);
}

MainWindow::~MainWindow()
{

}
