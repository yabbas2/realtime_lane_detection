#ifndef STREAM_H
#define STREAM_H

#include <QString>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <QtDBus>
#include <QApplication>
#include <QSharedMemory>
#include <chrono>
#include "ipm.h"

#define StreamingVideos     4
#define FRAME_SIZE          1152000

namespace stream {
enum {normal_rgb, final_rgb, ipm_rgb, ipm_bw};
}

using namespace cv;
using namespace std;
using namespace std::chrono;

class Stream : public QApplication
{
    Q_OBJECT

public:
    explicit Stream(int &argc, char **argv);
    ~Stream();
    void setInfo(vector<Vec2f> leftPts, vector<Vec2f> rightPts, Vec2i leftProp, Vec2i rightProp);
    Mat getFrame();
    void setIPMFrame(Mat *f);
    void setIPMBW(Mat *f);
    void changeStreamInSource(QString source);
    void pauseStream();
    void startStream();
    int width, height, fps;
    IPM ipm;

private slots:
    void loopOverFrames();
    void FullScreenFrame(int index);

private:
    QString streamInSource;
    cv::Mat inputFrame;
    cv::VideoCapture cap;
    Mat frames[StreamingVideos];
    Mat *fsFrame;
    Scalar leftColor;
    Scalar rightColor;
    vector<Vec2i> leftPts;
    vector<Vec2i> rightPts;
    bool updateDataLock;
    vector<Vec2i> bottomPts;
    vector<Vec2i> topPts;
    vector<vector<Point>> prevContours;
    vector<int> colorRange;
    QTimer *timer;
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *ifGUI;
    QDBusConnection bus2 = QDBusConnection::sessionBus();
    QDBusInterface *ifMaster;
    struct sharedData {
        uchar rawImg[FRAME_SIZE];
        uchar finalImg[FRAME_SIZE];
        uchar ipmImg[FRAME_SIZE];
        uchar ipmRGB[FRAME_SIZE];
    };
    QSharedMemory sm;
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;

    void drawFinalRGB();
    void reInitStream();
};

#endif // STREAM_H
