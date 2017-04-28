#ifndef STREAM_H
#define STREAM_H

#include <QString>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <vector>
#include "mainwindow.h"
#include "front-end/viewers/fullscreenvideoviewer.h"
#include "stream_in.h"
#include "stream_out.h"
#include "front-end/viewers/multivideoviewer.h"

#define StreamingVideos     4

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
    void FullScreenFrame(int index);

private:
    QString streamInSource;
    StreamIn stream_in;
    StreamOut stream_out;
    cv::Mat frames[StreamingVideos];
    cv::Mat *fsFrame;
    MultiVideoViewer *multiViewer;
    fullScreenVideoViewer *fsViewer;
    QTimer timer;
    int width, height, fps;
};

#endif // STREAM_H
