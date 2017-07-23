#ifndef IPM_H
#define IPM_H

#include <QString>
#include <vector>
#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QtDBus>
#include <chrono>
#include <QSharedMemory>
#include "../logger/logger.h"

#define IPM_FRAME_SIZE      1152000
#define FRAME_SIZE      1152000
#define frameHeight         480
#define frameWidth          800

using namespace cv;
using namespace std;
using namespace std::chrono;

class IPM : public QApplication
{
    Q_OBJECT

public:
    explicit IPM(int &argc, char **argv);
    void setVideoPts(QString);
    void ipmTransform();

    bool busy;

private:
    Mat inputFrame;
    Mat ipmFrame;
    Mat transformHomography;
    QSharedMemory sm;
    struct sharedData {
        uchar ipmData[IPM_FRAME_SIZE];
    };
    QSharedMemory sm2;
    struct sharedData2 {
        uchar rawImg[FRAME_SIZE];
    };
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *ifMaster;
    QDBusConnection bus2 = QDBusConnection::sessionBus();
    QDBusInterface *ifDetection;
    Logger log;
    int frameCount;
};

#endif // IPM_H
