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
    sidebar->setFixedSize(sideBarWidth, sideBarHeight);
    grid_main_layout->addWidget(sidebar, 0, 0);

    mulVidWidget = new MultiVideoViewer(cen1);
    mulVidWidget->setFixedSize(multiVideoWidth, mainWindowHeight);
    mulVidWidget->setVideoSize(200, 150);
    grid_main_layout->addWidget(mulVidWidget, 0, 1);

    fsVidWidget = new fullScreenVideoViewer(cen2);
    fsVidWidget->setFixedSize(mainWindowWidth, mainWindowHeight);
    fsVidWidget->setVideoSize(fsVidWidget->size());

    vidWid = new VideoWidget(cen2);
    vidWid->setFixedSize(videoWidgetWidth, mainWindowHeight);

    widgetStack = new QStackedWidget(this);
    widgetStack->insertWidget(0, cen1);
    widgetStack->insertWidget(1, cen2);
    widgetStack->setFixedSize(this->size());
    widgetStack->setCurrentIndex(0);

    connect(mulVidWidget->getVideoWidget(0), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
    connect(mulVidWidget->getVideoWidget(1), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
    connect(mulVidWidget->getVideoWidget(2), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
    connect(mulVidWidget->getVideoWidget(3), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
    connect(vidWid, SIGNAL(switchToMain()), this, SLOT(switchToMainScreen()));
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

Side_bar *MainWindow::getSideBarWidget()
{
    return sidebar;
}

void MainWindow::switchToFullScreen()
{
    widgetStack->setCurrentIndex(1);
}

void MainWindow::switchToMainScreen()
{
    widgetStack->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{

}
