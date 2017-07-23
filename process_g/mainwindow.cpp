#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)

{
    log.openFile(qApp->applicationDirPath() + "/../logger/logFiles/log_gui.txt", QIODevice::WriteOnly);
    log.write("----------------------------------------------------------");
    log.write("------------------------NEW RUN---------------------------");
    log.write("----------------------------------------------------------");
    this->setFixedSize(mainWindowWidth, mainWindowHeight);
//    this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
//    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setObjectName("mainWindow");

    cen = new QWidget(this);
    cen->setObjectName("cen2");
    cen->setFixedSize(this->size());

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
    QDBusReply<QString> key = ifMaster->call("getIPMSTREAMGUIKEY");
    sm.setKey(key.value());
    sm.attach(QSharedMemory::ReadOnly);
    QDBusReply<QString> key2 = ifMaster->call("getGUITRACKKEY");
    sm2.setKey(key2.value());
    sm2.attach(QSharedMemory::ReadOnly);
    QDBusReply<QString> key3 = ifMaster->call("getGUIREGKEY");
    sm3.setKey(key3.value());
    sm3.attach(QSharedMemory::ReadOnly);
    ifTrack = new QDBusInterface("com.stage.track", "/", "com.stage.track", bus3, this);
    ifIPM = new QDBusInterface("com.stage.ipm", "/", "com.stage.ipm", bus4, this);
    colorRange = {0, 0, 255};
    frameCount = 0;
    leftColor = Scalar(0, 0, 255);
    rightColor = Scalar(0, 0, 255);
    updateDataLock = true;
}

void MainWindow::callSetStreamSource(QString source)
{
    ifStream->call("setStreamSource", source);
    updateDataLock = true;
}

void MainWindow::callSetVideoName(QString source)
{
    ifIPM->call("setVideoName", source);
    ifTrack->call("setInvMat", source);
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
    t1 = high_resolution_clock::now();
    frameCount++;
    while (!sm.lock());
    sharedData *sData = (sharedData*) sm.data();
    globalFrame = Mat(frameHeight, frameWidth, CV_8UC3, sData->rawImg);
    sm.unlock();
    if (!updateDataLock)
        drawFinalRGB();
    fsVidWidget->getVideoWidget()->showImage(globalFrame);
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    log.write("[GUI] frame no. " + QString::number(frameCount) + ", exec time: " + QString::number(duration));
}

void MainWindow::updateData()
{
    updateDataLock = true;
    leftPts.clear();
    rightPts.clear();
    while (!sm2.lock());
    sharedData2 *sData2 = (sharedData2*) sm2.data();
    for (int i = 0; i < sData2->actualLeftSize; ++i)
        this->leftPts.push_back(Vec2i{(int) sData2->leftPts[i][0], (int) sData2->leftPts[i][1]});
    for (int i = 0; i < sData2->actualRightSize; ++i)
        this->rightPts.push_back(Vec2i{(int) sData2->rightPts[i][0], (int) sData2->rightPts[i][1]});
    sm2.unlock();
    if (rightPts.empty() || leftPts.empty())
        return;
    topPts.clear();
    bottomPts.clear();
    topPts.push_back(rightPts.at(0));
    bottomPts.push_back(rightPts.at(rightPts.size()-1));
    topPts.push_back(leftPts.at(0));
    bottomPts.push_back(leftPts.at(leftPts.size()-1));
    while (!sm3.lock());
    sharedData3 *sData3 = (sharedData3*) sm3.data();
    if (sData3->leftStatus[0] & sData3->leftStatus[1])
        leftColor = Scalar(0, 255, 0);
    else
        leftColor = Scalar(0, 0, 255);
    if (sData3->rightStatus[0] & sData3->rightStatus[1])
        rightColor = Scalar(0, 255, 0);
    else
        rightColor = Scalar(0, 0, 255);
    sm3.unlock();
    updateDataLock = false;
}

void MainWindow::drawFinalRGB()
{
    Mat processed = globalFrame.clone();
    Mat tmpFrame = globalFrame.clone();
    Mat mask;
    vector<vector<Point>> contours;
    polylines(processed, topPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, bottomPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, leftPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, rightPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(tmpFrame, leftPts, false, leftColor, 3, LINE_8);
    polylines(tmpFrame, rightPts, false, rightColor, 3, LINE_8);
    inRange(processed, colorRange, colorRange, mask);
    findContours(mask, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    if (contours.empty())
        contours = prevContours;
    if (contours.size())
    {
        fillConvexPoly(processed, contours[0], Scalar(200, 0, 0), LINE_AA);
        addWeighted(processed, 0.5, tmpFrame, 1 - 0.5, 0, tmpFrame);
        prevContours = contours;
    }
    globalFrame = tmpFrame.clone();
}

void MainWindow::initViewers()
{
    fsVidWidget->getVideoWidget()->showImage(defaultFrame);
}

MainWindow::~MainWindow()
{

}
