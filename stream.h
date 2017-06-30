#ifndef STREAM_H
#define STREAM_H

#include <QString>
#include <QObject>
#include <QTimer>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <vector>
#include <QtDBus>
#include <QApplication>
#include <QSharedMemory>
#include <chrono>
#include "../logger/logger.h"

#define FRAME_SIZE          1152000
#define IPM_FRAME_SIZE      1152000
#define frameHeight         480
#define frameWidth          800

using namespace cv;
using namespace std;
using namespace std::chrono;

class Stream : public QApplication
{
    Q_OBJECT

public:
    explicit Stream(int &argc, char **argv);
    ~Stream();
    void changeStreamInSource(QString source);
    void pauseStream();
    void startStream();
    int width, height, fps;
    void setVideoPts(QString);

private slots:
    void loopOverFrames();

private:
    QString streamInSource;
    Mat inputFrame;
    Mat ipmFrame;
    Mat transformHomography;
    VideoCapture cap;
    Mat fsFrame;
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
    QDBusConnection bus3 = QDBusConnection::sessionBus();
    QDBusInterface *ifDetection;
    QSharedMemory sm;
    struct sharedData {
        uchar rawImg[FRAME_SIZE];
    };
    QSharedMemory sm2;
    struct sharedData2 {
        uchar ipmData[IPM_FRAME_SIZE];
    };
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    Logger log;
    int frameCount;

    void drawFinalRGB();
    void reInitStream();
    void ipmTransform();
};

#endif // STREAM_H
