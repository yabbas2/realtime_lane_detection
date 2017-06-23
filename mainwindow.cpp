#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)

{
    this->setFixedSize(mainWindowWidth, mainWindowHeight);
//    this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
//    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setObjectName("mainWindow");

    cen = new QWidget(this);
    cen->setObjectName("cen2");
    cen->setFixedSize(this->size());

//    this->setCentralWidget(cen);
    fsVidWidget = new fullScreenVideoViewer(cen);
    fsVidWidget->setObjectName("fsVidWidget");
    fsVidWidget->setVideoSize(frameWidth, frameHeight);

    vidWid = new VideoWidget(cen);
    vidWid->setFixedSize(videoWidgetWidth, mainWindowHeight);

    defaultFrame = Mat::zeros(frameHeight, frameWidth, CV_8UC3);

    initViewers();

    ifStream = new QDBusInterface("com.stage.stream", "/", "com.stage.stream", bus, this);
    connect(vidWid, SIGNAL(pauseStreaming()), this, SLOT(callStopStream()));
    connect(vidWid, SIGNAL(startStreaming()), this, SLOT(callStartStream()));
    connect(vidWid, SIGNAL(changeVideoSource(QString)), this, SLOT(callSetStreamSource(QString)));
    connect(vidWid, SIGNAL(setVideoName(QString)), this, SLOT(callSetVideoName(QString)));

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
    globalFrame = Mat(frameHeight, frameWidth, CV_8UC3, sData->rawImg);
    sm.unlock();
    fsVidWidget->getVideoWidget()->showImage(globalFrame);
}

void MainWindow::initViewers()
{
    fsVidWidget->getVideoWidget()->showImage(defaultFrame);
}

MainWindow::~MainWindow()
{

}
