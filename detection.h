#ifndef LINEDETECTION_H
#define LINEDETECTION_H

#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QSharedMemory>
#include <QtDBus>

#define threshold_angle_min     70
#define threshold_angle_max     110
#define threshold_length        50
#define LINES_MAX_SIZE          50
#define IPM_FRAME_SIZE          1152000
#define frameHeight             480
#define frameWidth              800
#define DEBUG                   false

using namespace cv;
using namespace std;

namespace detectionSide {
    enum {left, right, both};
}

typedef Vec<int, 7> Vec7i;

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
    vector<int> boundary_max;
    vector<int> boundary_min;
    Ptr<LineSegmentDetector> lsd;
    vector<Vec4f> lines;
    vector<Vec7i> phaseOneFiltered;
    vector<Vec7i> phaseTwoFiltered;
    vector<Vec4f> tmpLeftLines;
    vector<Vec4f> tmpRightLines;
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *ifMaster;
    QSharedMemory sm;
    struct sharedData {
        int actualSize;
        int lineSegments[LINES_MAX_SIZE][7];
    };
    QSharedMemory sm2;
    struct sharedData2 {
        uchar ipmData[IPM_FRAME_SIZE];
    };

    void filterPhaseOne();
    void filterPhaseTwo();
};

#endif // LINEDETECTION_H
