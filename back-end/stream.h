#ifndef STREAM_H
#define STREAM_H

#include "stream_in.h"
#include "stream_out.h"
#include "front-end/multivideoviewer.h"
#include <QString>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "mainwindow.h"
#include <vector>

class Stream : public QObject
{
    Q_OBJECT

public:
    Stream();
    void connectToFrontEnd(MainWindow *w);
    void setPointsToDraw(std::vector<cv::Vec2i> *pts);

public slots:
    void changeStreamInSource(QString source);

private slots:
    void showFrames();

private:
    QString streamInSource;
    StreamIn stream_in;
    StreamOut stream_out;
    cv::Mat *normalFrame;
    cv::Mat *finalFrameRGB;
    cv::Mat *ipmFrameRGB;
    cv::Mat *ipmFrameBW;
    MultiVideoViewer *multiViewer;
    QTimer timer;
    int width, height, fps;
};

#endif // STREAM_H
