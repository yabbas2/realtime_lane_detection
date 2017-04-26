#ifndef PIPELINE_H
#define PIPELINE_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QTimer>
#include "stream.h"
#include "ipm.h"
#include "front-end/viewers/fullscreenvideoviewer.h"
#include "front-end/viewers/multivideoviewer.h"
#include "front-end/sidebar/side_bar.h"
#include "front-end/videowidget.h"
#include "mainwindow.h"

using namespace std;
using namespace cv;

class Pipeline : public QObject
{
    Q_OBJECT

public:
    explicit Pipeline();
    ~Pipeline();
    void connectFrontEndToBackEnd(MainWindow *w);

private slots:
    void start_timers();
    void pause_timers();
    void exec();
    void initStatus();

private:
    Stream *streamObj;
    Mat normalFrame;
    IPM *ipmObj;
    Mat *ipmFrame;
    MultiVideoViewer *multiViewer;
    fullScreenVideoViewer *fsViewer;
    VideoWidget *videoWidget;
    SideBar *sideBar;
    QTimer *timer;
};

#endif // PIPELINE_H
