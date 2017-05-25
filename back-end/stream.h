#ifndef STREAM_H
#define STREAM_H

#include <QString>
#include <QDebug>
#include <QObject>
#include <opencv2/opencv.hpp>
#include <vector>
#include <QThread>
#include "front-end/viewers/fullscreenvideoviewer.h"
#include "front-end/viewers/multivideoviewer.h"

#define StreamingVideos     4

using namespace std;
using namespace cv;

class Stream : public QThread
{
    Q_OBJECT

public:
    explicit Stream();
    ~Stream();
    void setViewers(MultiVideoViewer *m, fullScreenVideoViewer *f);
    void setInfo(vector<Vec2f> leftPts, vector<Vec2f> rightPts, Vec2i leftProp, Vec2i rightProp);
    Mat getFrame();
    void setIPMFrame(Mat *f);
    void setIPMBW(Mat *f);
    void pauseStream();
    void startStream();
    void reInitStream();
    int width, height, fps;

protected:
    void run();
//    void msleep(int ms);

public slots:
    void changeStreamInSource(QString source);
    void initScreens();

private slots:
//    void showFrames();
    void FullScreenFrame(int index);
    void stopStream();

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
    vector<Vec2i> leftPts;
    vector<Vec2i> rightPts;
    bool updateDataLock;
    bool updateFrame;
    vector<Vec2i> bottomPts;
    vector<Vec2i> topPts;
    vector<vector<Point>> prevContours;
    vector<int> colorRange;

    void drawFinalRGB();
};

#endif // STREAM_H
