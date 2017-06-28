#ifndef LINEDETECTION_H
#define LINEDETECTION_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QSharedMemory>
#include <QtDBus>
#include <chrono>
#include "../logger/logger.h"

#define LINES_MAX_SIZE          100
#define IPM_FRAME_SIZE          1152000
#define frameHeight             480
#define frameWidth              800
#define DEBUG                   false

using namespace cv;
using namespace std;
using namespace std::chrono;

namespace detectionSide {
    enum {left, right, both};
}

class Detection : public QApplication
{
public:
    Detection(int &argc, char **argv);
    void lineSegmentDetector();
    void cannyHough(int side);
    bool busy;

private:
    Mat inputFrame;
    Mat processedFrame;
    Ptr<LineSegmentDetector> lsd;
    vector<Vec4f> lines;
    vector<Vec4f> tmpLeftLines;
    vector<Vec4f> tmpRightLines;
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *ifMaster;
    QSharedMemory sm;
    struct sharedData {
        int actualSize;
        int lineSegments[LINES_MAX_SIZE][4];
        uchar ipmData[IPM_FRAME_SIZE];
    };
    QDBusConnection bus2 = QDBusConnection::sessionBus();
    QDBusInterface *ifReg;
    QSharedMemory sm2;
    struct sharedData2 {
        uchar ipmData[IPM_FRAME_SIZE];
    };
    Logger log;
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    int frameCount;
};

#endif // LINEDETECTION_H
