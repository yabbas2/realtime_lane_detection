#include "stream_in.h"

StreamIn::StreamIn(QObject *parent) :
    QObject(parent)
{

}

void StreamIn::initStreamIn(QString source, QThread &thread)
{
    if (!cap.isOpened())
        if (!cap.open(source.toStdString()))
        {
            qDebug() << "Cannot open video streaming source";
            return;
        }
    connect(&thread, SIGNAL(started()), this, SLOT(startStreamIn()));
    connect(&thread, SIGNAL(finished()), this, SLOT(stopStreamIn()));
    connect(this, SIGNAL(endStream()), this, SLOT(stopStreamIn()));
}

void StreamIn::startStreamIn()
{
    qDebug() << "start streaming";
    while (true)
    {
        cap >> inputFrame;
        if (!cap.grab())
        {
            emit endStream();
            break;
        }
    }
}

void StreamIn::stopStreamIn()
{
    qDebug() << "end streaming";
    cap.release();
}

StreamIn::~StreamIn()
{

}
