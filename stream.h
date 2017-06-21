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
#include "viewers/multivideoviewer.h"

#define StreamingVideos     4

using namespace cv;
using namespace std;

class Stream : public QObject
{
    Q_OBJECT

public:
    explicit Stream();
    ~Stream();
    void setInfo(vector<Vec2f> leftPts, vector<Vec2f> rightPts, Vec2i leftProp, Vec2i rightProp);
    Mat getFrame();
    void setIPMFrame(Mat *f);
    void setIPMBW(Mat *f);
    void reInitStream();
    void connectToFrontEnd(MainWindow *w);
    int width, height, fps;

public slots:
    void changeStreamInSource(QString source);
    void initScreens();
    void pauseStream();
    void startStream();

private slots:
    void showFrames();
    void FullScreenFrame(int index);

private:
    QString streamInSource;
    cv::Mat inputFrame;
    cv::VideoCapture cap;
    Mat frames[StreamingVideos];
    Mat *fsFrame;
    Scalar leftColor;
    Scalar rightColor;
    Mat normal_default_screen;
    Mat ipm_default_screen;
    MultiVideoViewer *multiViewer;
    fullScreenVideoViewer *fsViewer;
    SideBar *sideBar;
    VideoWidget *videoWidget;
    vector<Vec2i> leftPts;
    vector<Vec2i> rightPts;
    bool updateDataLock;
    bool updateFrame;
    vector<Vec2i> bottomPts;
    vector<Vec2i> topPts;
    vector<vector<Point>> prevContours;
    vector<int> colorRange;
    QTimer *timer;

    void drawFinalRGB();

};

#endif // STREAM_H
