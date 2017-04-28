#include "stream_in.h"

StreamIn::StreamIn(QObject *parent) :
    QObject(parent)
{

}

void StreamIn::initStreamIn(QString source)
{
    if (timer.isActive())
        timer.stop();
    if (cap.isOpened())
        cap.release();
    if (!cap.open(source.toStdString()))
    {
        qDebug() << "Cannot open video streaming source";
        return;
    }
    int msec = static_cast<int> (1000 / cap.get(cv::CAP_PROP_FPS));
    connect(this, SIGNAL(endStream()), this, SLOT(stopStreamIn()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(startStreamIn()));
    timer.start(msec + 1);
    qDebug() << "start streaming";
}

void StreamIn::startStreamIn()
{
    cap >> inputFrame;
    if (!cap.grab())
        emit endStream();
}

void StreamIn::stopStreamIn()
{
    timer.stop();
    qDebug() << "end streaming";
    cap.release();
}

void StreamIn::getVideoInfo(int &w, int &h, int &fps)
{
    w = static_cast<int> (cap.get(cv::CAP_PROP_FRAME_WIDTH));
    h = static_cast<int> (cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    fps = static_cast<int> (cap.get(cv::CAP_PROP_FPS));
}

cv::Mat *StreamIn::getFrame()
{
    return &inputFrame;
}

StreamIn::~StreamIn()
{

}
