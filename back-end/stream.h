#ifndef STREAM_H
#define STREAM_H

#include <QString>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <vector>
#include "front-end/viewers/fullscreenvideoviewer.h"
#include "stream_in.h"
#include "stream_out.h"
#include "front-end/viewers/multivideoviewer.h"

#define StreamingVideos     4

class Stream : public QObject
{
    Q_OBJECT

public:
    explicit Stream();
    ~Stream();
    void setViewers(MultiVideoViewer *m, fullScreenVideoViewer *f);
    void setPointsToDraw(std::vector<cv::Vec2f> leftPoints, std::vector<cv::Vec2f> rightPoints);
    cv::Mat getFrame();
    void setIPMFrame(cv::Mat *f);
    void pause_timers();
    void start_timers();
    void reInitStream();
    int width, height, fps;

public slots:
    void changeStreamInSource(QString source);
    void initScreens();

private slots:
    void showFrames();
    void FullScreenFrame(int index);

private:
    QString streamInSource;
    StreamIn *stream_in;
    StreamOut *stream_out;
    cv::Mat frames[StreamingVideos];
    cv::Mat *fsFrame;
    cv::Mat normal_default_screen;
    cv::Mat ipm_default_screen;
    MultiVideoViewer *multiViewer;
    fullScreenVideoViewer *fsViewer;
    QTimer *timer;
    std::vector<cv::Vec2i> leftPoints;
    std::vector<cv::Vec2i> rightPoints;
    bool updateDataLock;
};

#endif // STREAM_H
