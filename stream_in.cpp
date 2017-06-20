#include "stream_in.h"

StreamIn::StreamIn()
{
    timer = new QTimer(this);
    connect(this, SIGNAL(endStream()), this, SLOT(stopStreamIn()));
    connect(timer, SIGNAL(timeout()), this, SLOT(loopStreamIn()));
}

void StreamIn::initStreamIn(QString source)
{
    if (timer->isActive())
        timer->stop();
    if (cap.isOpened())
        cap.release();
    if (!cap.open(source.toStdString()))
    {
        qDebug() << "[STREAM_IN] Cannot open video streaming source";
        return;
    }
}

void StreamIn::loopStreamIn()
{
    cap >> inputFrame;
    if (!cap.grab() || inputFrame.cols == 0 || inputFrame.rows == 0)
        emit endStream();
}

void StreamIn::startStreamIn()
{
    if (timer->isActive())
        return;
    timer->start(static_cast<int> (1000 / cap.get(cv::CAP_PROP_FPS)) + delayOffset);
    qDebug() << "[STREAM_IN] start streaming";
}

void StreamIn::stopStreamIn()
{
    if (!timer->isActive() && !cap.isOpened())
        return;
    timer->stop();
    qDebug() << "[STREAM_IN] stop streaming";
    cap.release();
}

void StreamIn::pauseStreamIn()
{
    if (!timer->isActive())
        return;
    timer->stop();
    qDebug() << "[STREAM_IN] pause streaming";
}

void StreamIn::getVideoInfo(int &w, int &h, int &fps)
{
    w = static_cast<int> (cap.get(cv::CAP_PROP_FRAME_WIDTH));
    h = static_cast<int> (cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    fps = static_cast<int> (cap.get(cv::CAP_PROP_FPS));
}

cv::Mat *StreamIn::getFrame()
{
    if (inputFrame.cols != 800 || inputFrame.rows != 480)
        cv::resize(inputFrame, inputFrame, cv::Size(800, 480), 0, 0, cv::INTER_LINEAR);
    return &inputFrame;
}

StreamIn::~StreamIn()
{

}
