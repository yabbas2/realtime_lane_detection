#ifndef PIPELINE_H
#define PIPELINE_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QTimer>
#include <vector>
#include <QDebug>
#include <QString>
#include <iostream>
#include "stream.h"
#include "ipm.h"
#include "filter.h"
#include "kalman.h"
#include "decision_making.h"
#include "linedetection.h"
#include "region_growing.h"
#include "curvefit.h"
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
    void connectToStreamEngine(Stream *s);

private slots:
    void start_timers();
    void pause_timers();
    void exec();
    void initStatus();
    void setVideoSource(QString s);

private:
    Stream *streamObj;
    IPM *ipmObj;
    LineDetection *lineDetector;
    Filter *filter;
    RegionGrowing *regGrow;
    CurveFit *curveFit;
    Kalman *k;
    DecisionMaking *decisionMake;

    Mat normalFrame;
    Mat *ipmFrame;
    Mat magdy;
    Mat _3ebs;
    vector<Vec4f> *detectedLines;
    vector<Vec7i> *filteredLines;
    vector<Vec7i> *leftRegion;
    vector<Vec7i> *rightRegion;
    vector<Vec2f> *leftPoints;
    vector<Vec2f> *rightPoints;
    vector<Vec2f> emptyPoints;
    Vec7i *leftSeedLine;
    Vec7i *rightSeedLine;

    MultiVideoViewer *multiViewer;
    fullScreenVideoViewer *fsViewer;
    VideoWidget *videoWidget;
    SideBar *sideBar;
    QTimer *timer;

    Vec2i leftStatus;
    Vec2i rightStatus;

    QString videoName;

    void redetectLines(int side);
};

#endif // PIPELINE_H
