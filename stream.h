#ifndef STREAM_H
#define STREAM_H

#include <QString>
#include <QDebug>
#include <QObject>
#include "mainwindow.h"
#include <QTimer>
#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include "sidebar/side_bar.h"
#include "videowidget.h"
#include <vector>
#include "viewers/fullscreenvideoviewer.h"
#include "stream_in.h"
#include "stream_out.h"
#include "viewers/multivideoviewer.h"

#define StreamingVideos     4

class Stream : public QObject
{
    Q_OBJECT

public:
    explicit Stream();
    ~Stream();
    void setViewers(MultiVideoViewer *m, fullScreenVideoViewer *f);
    void setInfo(std::vector<cv::Vec2f> leftPoints, std::vector<cv::Vec2f> rightPoints, Vec2i ls, Vec2i rs);
    cv::Mat getFrame();
    void setIPMFrame(cv::Mat *f);
    void setIPMBW(cv::Mat *f);

    void reInitStream();
    void connectToFrontEnd(MainWindow *w);
    int width, height, fps;

public slots:
    void changeStreamInSource(QString source);
    void pause_timers();
    void start_timers();
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
    QTimer *timer;
    std::vector<cv::Vec2i> leftPoints;
    std::vector<cv::Vec2i> rightPoints;
    MultiVideoViewer *multiViewer;
    fullScreenVideoViewer *fsViewer;
    SideBar *sideBar;
    VideoWidget *videoWidget;
    bool updateDataLock;
};

#endif // STREAM_H
