#ifndef REG_H
#define REG_H

#include <opencv2/core.hpp>
#include <vector>
#include <QApplication>
#include <QSharedMemory>
#include <QtDBus>
#include <libalglib/stdafx.h>
#include <libalglib/interpolation.h>
#include <chrono>
#include "../logger/logger.h"

#define USED                    1
#define NOT_USED                0
#define thresholdAngle          25
#define thresholdX              40
#define frameWidth              480
#define frameHeight             800
#define overlapThreshold        180
#define outOfRangeThreshold     500
#define PTS_NUM                 20
#define START_Y                 0
#define END_Y                   frameHeight
#define LINES_MAX_SIZE          50
#define yThreshold1             150
#define yThreshold2             10
#define xThreshold2             100
#define IPM_FRAME_SIZE          1152000

using namespace std;
using namespace cv;
using namespace alglib;
using namespace std::chrono;

typedef Vec<int, 7> Vec7i;

namespace REG {
    enum {left, right, both};
}

class Reg : public QApplication
{
    Q_OBJECT

public:
    explicit Reg(int &argc, char **argv);
    void process();

    bool busy;

private:
    Mat test;
    vector<Vec7i> leftLines;
    vector<Vec7i> rightLines;
    Vec7i leftSeedLine;
    Vec7i rightSeedLine;
    vector<Vec7i> leftRegion;
    vector<Vec7i> rightRegion;
    real_1d_array leftXBeforeFit;
    real_1d_array leftYBeforeFit;
    real_1d_array rightXBeforeFit;
    real_1d_array rightYBeforeFit;
    vector<float> newPtsY;
    vector<Vec2f> leftPtsAfterFit;
    vector<Vec2f> rightPtsAfterFit;
    bool leftState, rightState;
    QSharedMemory sm;
    struct sharedData {
        int actualSize;
        int lineSegments[LINES_MAX_SIZE][6];
    };
    QSharedMemory sm2;
    struct sharedData2 {
        int actualLeftSize;
        int actualRightSize;
        float leftPts[PTS_NUM][2];
        float rightPts[PTS_NUM][2];
    };
    QSharedMemory sm4;
    struct sharedData4 {
        int leftStatus[2];
        int rightStatus[2];
    };
    Logger log;
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *ifMaster;
    QDBusConnection bus2 = QDBusConnection::sessionBus();
    QDBusInterface *ifTrack;
    QDBusConnection bus3 = QDBusConnection::sessionBus();
    QDBusInterface *ifGUI;
    int frameCount;
    Vec2i leftStatus;
    Vec2i rightStatus;
    QSharedMemory sm3;
    struct sharedData3 {
        uchar ipmData[IPM_FRAME_SIZE];
    };

    void makeLinspace();
    void fromLinesToPoints(int side);
    void genSeedLine(int c);
    void findLeftSeedLine();
    void findRightSeedLine();
    void genRegGrow(int c);
    void regionGrowing(int option);
    void curveFit(int side);
    void validateLineBefore(int side);
    void validateLineAfter();
    void decideType(int side);
    void decideColor(int side);
};

#endif // REG_H
