#include "stream.h"

Stream::Stream(int &argc, char **argv) :
    QApplication(argc, argv),
    width(0), height(0), fps(0)
{
    log.openFile(qApp->applicationDirPath() + "/../logger/logFiles/log_stream.txt", QIODevice::WriteOnly);
    log.write("----------------------------------------------------------");
    log.write("------------------------NEW RUN---------------------------");
    log.write("----------------------------------------------------------");
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(loopOverFrames()));
    frameCount = 0;
    ifGUI = new QDBusInterface("com.stage.gui", "/", "com.stage.gui", bus, this);
    ifMaster = new QDBusInterface("com.stage.master", "/", "com.stage.master", bus2, this);
    ifIPM = new QDBusInterface("com.stage.ipm", "/", "com.stage.ipm", bus3, this);
    QDBusReply<QString> key = ifMaster->call("getIPMSTREAMGUIKEY");
    sm.setKey(key.value());
    sm.attach(QSharedMemory::ReadWrite);
}

void Stream::changeStreamInSource(QString source)
{
    reInitStream();
    streamInSource = source;
    if (!cap.open(source.toStdString()))
    {
        log.write("[STREAM] Cannot open video streaming source - exiting..");
        exit(1);
    }
    fps = static_cast<int> (cap.get(cv::CAP_PROP_FPS));
    int frameCount = static_cast<int> (cap.get(cv::CAP_PROP_FRAME_COUNT));
    log.write("[STREAM] opened (" + source + ") - fps: " + QString::number(fps) + " - frameCount: " + QString::number(frameCount/2));
}


void Stream::reInitStream()
{
    if (cap.isOpened())
        cap.release();
    timer->stop();
    ifGUI->call("initViewers");
    log.write("[STREAM] pause/stop streaming");
}

void Stream::loopOverFrames()
{
    t1 = high_resolution_clock::now();
    cap >> inputFrame;
    if (!cap.grab() || inputFrame.cols == 0 || inputFrame.rows == 0)
    {
        reInitStream();
        timer->stop();
        return;
    }
    frameCount++;
    if (inputFrame.cols != 800 || inputFrame.rows != 480)
        resize(inputFrame, inputFrame, Size(800, 480), 0, 0, INTER_LINEAR);
    uchar *rawData = inputFrame.data;
    while (!sm.lock());
    sharedData *sData = (sharedData*) sm.data();
    memcpy(sData->rawImg, rawData, FRAME_SIZE);
    sm.unlock();
    ifGUI->call("showFrames");
    ifIPM->call("ipmTransform");
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    timer->setInterval(((1000 / fps) > duration) ? ((1000 / fps) - duration) : 0);
    log.write("[STREAM] frame no. " + QString::number(frameCount) + " - timer interval: " + QString::number(timer->interval()));
}

void Stream::pauseStream()
{
    log.write("[STREAM] pause/stop streaming");
    timer->stop();
}

void Stream::startStream()
{
    log.write("[STREAM] start/continue streaming");
    timer->start(static_cast<int> (1000/fps));
}

Stream::~Stream()
{
}
