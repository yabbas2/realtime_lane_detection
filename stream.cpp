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
    updateDataLock = true;
    frameCount = 0;
    colorRange = {0, 0, 255};
    ifGUI = new QDBusInterface("com.stage.gui", "/", "com.stage.gui", bus, this);
    ifMaster = new QDBusInterface("com.stage.master", "/", "com.stage.master", bus2, this);
    ifDetection = new QDBusInterface("com.stage.detection", "/", "com.stage.detection", bus3, this);
    QDBusReply<QString> key = ifMaster->call("getSTREAMGUIKEY");
    sm.setKey(key.value());
    sm.attach(QSharedMemory::ReadWrite);
    QDBusReply<QString> key2 = ifMaster->call("getSTREAMDETECTIONKEY");
    sm2.setKey(key2.value());
    sm2.attach(QSharedMemory::ReadWrite);
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
    fsFrame = inputFrame.clone();
    uchar *rawData = fsFrame.data;
    while (!sm.lock());
    sharedData *sData = (sharedData*) sm.data();
    memcpy(sData->rawImg, rawData, FRAME_SIZE);
    sm.unlock();
    ifGUI->call("showFrames");
    ipmTransform();
    uchar *ipmData = ipmFrame.data;
    while (!sm2.lock());
    sharedData2 *sData2 = (sharedData2*) sm2.data();
    memcpy(sData2->ipmData, ipmData, IPM_FRAME_SIZE);
    sm2.unlock();
    ifDetection->call("detect");
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

void Stream::drawFinalRGB()
{
    Mat processed = fsFrame.clone();
    Mat mask;
    vector<vector<Point>> contours;
    topPts.clear();
    bottomPts.clear();
    topPts.push_back(rightPts.at(0));
    bottomPts.push_back(rightPts.at(rightPts.size()-1));
    topPts.push_back(leftPts.at(0));
    bottomPts.push_back(leftPts.at(leftPts.size()-1));
    polylines(processed, topPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, bottomPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, leftPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, rightPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(fsFrame, leftPts, false, leftColor, 3, LINE_8);
    polylines(fsFrame, rightPts, false, rightColor, 3, LINE_8);
    inRange(processed, colorRange, colorRange, mask);
    findContours(mask, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    if (contours.empty())
        contours = prevContours;
    if (contours.size())
    {
        fillConvexPoly(processed, contours[0], Scalar(200, 0, 0), LINE_AA);
        addWeighted(processed, 0.5, fsFrame, 1 - 0.5, 0, fsFrame);
        prevContours = contours;
    }
}

void Stream::ipmTransform()
{
    warpPerspective(inputFrame, ipmFrame, transformHomography, Size(frameHeight, frameWidth));
}

void Stream::setVideoPts(QString videoName)
{
    Mat inputPts;
    Mat dstPts;
    dstPts = (Mat_<double>(4,2) << 0, 0, frameHeight, 0, frameHeight, frameWidth, 0, frameWidth);
    if (videoName == "youtube_video1")
        inputPts = (Mat_<double>(4, 2) << 325, 280, 485, 280, 748, 478, 120, 478);
    else if (videoName == "youtube_video2" || videoName == "youtube_video3")
        inputPts = (Mat_<double>(4, 2) << 340, 280, 520, 280, 770, 478, 120, 478);
    else if (videoName == "youtube_video4")
        inputPts = (Mat_<double>(4, 2) << 310, 280, 445, 280, 615, 390, 200, 390);
    else if (videoName == "youtube_video5")
        inputPts = (Mat_<double>(4, 2) << 365, 300, 490, 300, 700, 440, 210, 440);
    else if (videoName == "youtube_video6")
        inputPts = (Mat_<double>(4, 2) << 380, 220, 480, 220, 610, 350, 235, 350);
    else if (videoName.startsWith("kitti_video"))
        inputPts = (Mat_<double>(4, 2) << 330, 300, 460, 300, 600, 478, 200, 478);
    else if (videoName.startsWith("udacity_video"))
        inputPts = (Mat_<double>(4, 2) << 340, 310, 475, 310, 740, 430, 120, 430);
    inputPts.convertTo(inputPts, CV_32F);
    dstPts.convertTo(dstPts, CV_32F);
    transformHomography = getPerspectiveTransform(inputPts, dstPts);
}

Stream::~Stream()
{
}
