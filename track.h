#ifndef TRACK_H
#define TRACK_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <QApplication>
#include <QSharedMemory>
#include <QtDBus>
#include <chrono>
#include <opencv2/video/tracking.hpp>
#include "../logger/logger.h"

#define PTS_NUM                 20

using namespace std;
using namespace cv;
using namespace std::chrono;

namespace TRACK {
enum {left, right};
}

class Track : public QApplication
{
    Q_OBJECT

public:
    explicit Track(int &argc, char **argv);
    void process();

    bool busy;

private:
    Logger log;
    int frameCount;
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *ifMaster;
    QSharedMemory sm2;
    struct sharedData2 {
        int actualLeftSize;
        int actualRightSize;
        float leftPts[PTS_NUM][2];
        float rightPts[PTS_NUM][2];
    };
    KalmanFilter leftKalman[20];
    KalmanFilter rightKalman[20];
    vector<Vec2f> inputLeftPts;
    vector<Vec2f> inputRightPts;
    vector<Vec2f> prevLeftPts;
    vector<Vec2f> prevRightPts;
    vector<Vec2f> newLeftPts;
    vector<Vec2f> newRightPts;
    vector<Vec2f> *inputPts;
    vector<Vec2f> *newPts;
    vector<Vec2f> *prevPts;
    KalmanFilter* k;
    Mat mp;
    Mat tp;
    int isMeasure;
    int failCount;
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;

    void smooth();
    void leftSmoothing();
    void rightSmoothing();
    void kalmanFilter(int side);
    void initFilter();
};

#endif // TRACK_H
