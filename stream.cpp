#include "stream.h"

Stream::Stream(int &argc, char **argv) :
    QApplication(argc, argv),
    width(0), height(0), fps(0)
{
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(loopOverFrames()));
    updateDataLock = true;
    colorRange = {0, 0, 255};
    ifGUI = new QDBusInterface("com.stage.gui", "/", "com.stage.gui", bus, this);
    ifMaster = new QDBusInterface("com.stage.master", "/", "com.stage.master", bus2, this);
    QDBusReply<QString> key = ifMaster->call("getSTREAMGUIKEY");
    sm.setKey(key.value());
    sm.attach(QSharedMemory::ReadWrite);
}

void Stream::changeStreamInSource(QString source)
{
    reInitStream();
    streamInSource = source;
    if (!cap.open(source.toStdString()))
    {
        qDebug() << "[STREAM] Cannot open video streaming source";
        exit(1);
    }
    fps = static_cast<int> (cap.get(cv::CAP_PROP_FPS));
    qDebug() << "[STREAM] using framerate:" << fps;
}


void Stream::reInitStream()
{
    if (cap.isOpened())
        cap.release();
    timer->stop();
    ifGUI->call("initViewers");
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
    if (inputFrame.cols != 800 || inputFrame.rows != 480)
        resize(inputFrame, inputFrame, Size(800, 480), 0, 0, INTER_AREA);
    fsFrame = inputFrame.clone();
//    if (! updateDataLock)
//    {
//        drawFinalRGB();
//    }
    ipm.transform(fsFrame);
    uchar *rawData = fsFrame.data;
    sm.lock();
    sharedData *sData = (sharedData*) sm.data();
    memcpy(sData->rawImg, rawData, FRAME_SIZE);
    sm.unlock();
    ifGUI->call("showFrames");
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    if ((1000/fps) > duration)
        timer->setInterval((1000/fps) - duration);
    else
        timer->setInterval(0);
}

void Stream::pauseStream()
{
    qDebug() << "[STREAM] pause/stop streaming";
    timer->stop();
}

void Stream::startStream()
{
    qDebug() << "[STREAM] start/continue streaming";
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

void Stream::setInfo(vector<Vec2f> leftPoints, vector<Vec2f> rightPoints, Vec2i leftProp, Vec2i rightProp)
{
    updateDataLock = true;
    if (leftProp[0] & leftProp[1])
        leftColor = Scalar(0, 255, 0);
    else
        leftColor = Scalar(0, 0, 255);
    if (rightProp[0] & rightProp[1])
        rightColor = Scalar(0, 255, 0);
    else
        rightColor = Scalar(0, 0, 255);
    this->leftPts.clear();
    this->rightPts.clear();
    for (unsigned int i = 0; i < leftPoints.size(); i++)
        this->leftPts.push_back(Vec2i{(int)leftPoints[i][0], (int)leftPoints[i][1]});
    for (unsigned int i = 0; i < rightPoints.size(); i++)
        this->rightPts.push_back(Vec2i{(int)rightPoints[i][0], (int)rightPoints[i][1]});
    updateDataLock = false;
}

Stream::~Stream()
{

}
