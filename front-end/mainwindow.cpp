#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)

{
    this->setFixedSize(mainWindowWidth, mainWindowHeight);
//    this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
//    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setObjectName("mainWindow");

    h_cen1_layout = new QHBoxLayout;
    cen1 = new QWidget(this);
    cen1->setObjectName("cen1");
    cen2 = new QWidget(this);
    cen1->setObjectName("cen2");
    cen1->setFixedSize(this->size());
    cen2->setFixedSize(this->size());

    cen1->setLayout(h_cen1_layout);

    sidebar = new SideBar(cen1);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedSize(sideBarWidth, sideBarHeight);
    h_cen1_layout->addWidget(sidebar);

    mulVidWidget = new MultiVideoViewer(cen1);
    mulVidWidget->setObjectName("mulVidWidget");
    mulVidWidget->setFixedSize(multiVideoWidth, multiVideoHeight);
    mulVidWidget->setVideoSize(220, 140);
    h_cen1_layout->addWidget(mulVidWidget);

    fsVidWidget = new fullScreenVideoViewer(cen2);
    fsVidWidget->setObjectName("fsVidWidget");


    vidWid = new VideoWidget(cen2);
    vidWid->setFixedSize(videoWidgetWidth, mainWindowHeight);

    widgetStack = new QStackedWidget(this);
    widgetStack->insertWidget(0, cen1);
    widgetStack->insertWidget(1, cen2);
    widgetStack->setFixedSize(this->size());
    widgetStack->setCurrentIndex(0);

    connect(mulVidWidget->getVideoWidget(0), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen(int)));
    connect(mulVidWidget->getVideoWidget(1), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen(int)));
    connect(mulVidWidget->getVideoWidget(2), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen(int)));
    connect(mulVidWidget->getVideoWidget(3), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen(int)));
    connect(vidWid, SIGNAL(switchToMain()), this, SLOT(switchToMainScreen()));

    this->setStyleSheet("#mainWindow {border-image: url(:/images/resources/images/background.png) 0 0 0 0 stretch stretch; border-width: 0px; border-radius: 0px;}"
                        "#sidebar {background: rgba(100, 100, 100, 50); border: 1px solid gray; border-radius: 10px;}"
                        "#mulVidWidget {background: rgba(100, 100, 100, 50); border: 1px solid gray; border-radius: 10px;}");

    normal_default_screen = cv::Mat::zeros(frameHeight, frameWidth, CV_8UC3);
    ipm_default_screen = cv::Mat::zeros(frameWidth, frameHeight, CV_8UC3);
    initViewers();

    ifStream = new QDBusInterface("com.stage.stream", "/", "com.stage.stream", bus, this);
    connect(vidWid, SIGNAL(pauseStreaming()), this, SLOT(callStopStream()));
    connect(vidWid, SIGNAL(startStreaming()), this, SLOT(callStartStream()));
    connect(sidebar->inputMethod, SIGNAL(pauseStreaming()), this, SLOT(callStopStream()));
    connect(sidebar->inputMethod, SIGNAL(startStreaming()), this, SLOT(callStartStream()));
    connect(sidebar->inputMethod, SIGNAL(changeVideoSource(QString)), this, SLOT(callSetStreamSource(QString)));
    connect(sidebar->inputMethod, SIGNAL(setVideoName(QString)), this, SLOT(callSetVideoName(QString)));

    ifMaster = new QDBusInterface("com.stage.master", "/", "com.stage.master", bus2, this);
    QDBusReply<QString> key = ifMaster->call("getSTREAMGUIKEY");
    sm.setKey(key.value());
    sm.attach(QSharedMemory::ReadOnly);
}

void MainWindow::callSetStreamSource(QString source)
{
    ifStream->call("setStreamSource", source);
}

void MainWindow::callSetVideoName(QString source)
{
    ifStream->call("setVideoName", source);
}

void MainWindow::callStartStream()
{
    ifStream->call("startStream");
}

void MainWindow::callStopStream()
{
    ifStream->call("stopStream");
}

void MainWindow::showFrames()
{
    sm.lock();
    sharedData *sData = (sharedData*) sm.data();
    normal_rgb_frame = Mat(frameHeight, frameWidth, CV_8UC3, sData->rawImg);
    final_rgb_frame = Mat(frameHeight, frameWidth, CV_8UC3, sData->finalImg);
    ipm_frame = Mat(frameWidth, frameHeight, CV_8UC3, sData->ipmImg);
    ipm_rgb_frame = Mat(frameWidth, frameHeight, CV_8UC3, sData->ipmRGB);
    sm.unlock();
    mulVidWidget->getVideoWidget(0)->showImage(normal_rgb_frame);
    mulVidWidget->getVideoWidget(1)->showImage(final_rgb_frame);
    mulVidWidget->getVideoWidget(2)->showImage(ipm_frame);
    mulVidWidget->getVideoWidget(3)->showImage(ipm_rgb_frame);
}

void MainWindow::initViewers()
{
    mulVidWidget->getVideoWidget(0)->showImage(normal_default_screen);
    mulVidWidget->getVideoWidget(1)->showImage(normal_default_screen);
    mulVidWidget->getVideoWidget(2)->showImage(ipm_default_screen);
    mulVidWidget->getVideoWidget(3)->showImage(ipm_default_screen);
    fsVidWidget->getVideoWidget()->showImage(normal_default_screen);
}


void MainWindow::switchToFullScreen(int index)
{
    widgetStack->setCurrentIndex(1);
    switch(index)
       {
       case 0:
       case 1:
           fsVidWidget->setGeometry(0, 0, 854, 480);
           fsVidWidget->setVideoSize(854, 480);
           break;
       case 2:
       case 3:
           fsVidWidget->setGeometry(292, 0, 270, 480);
           fsVidWidget->setVideoSize(270, 480);
           break;
       }
}

void MainWindow::switchToMainScreen()
{
    widgetStack->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{

}
