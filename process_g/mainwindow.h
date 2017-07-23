#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtDBus>
#include <QSharedMemory>
#include <opencv2/opencv.hpp>
#include <vector>
#include <chrono>
#include "fullscreenvideoviewer.h"
#include "videowidget.h"
#include "../logger/logger.h"

#define mainWindowWidth     800
#define mainWindowHeight    480
#define videoWidgetWidth    300
#define FRAME_SIZE          1152000
#define frameHeight         480
#define frameWidth          800
#define PTS_NUM             20

using namespace cv;
using namespace std;
using namespace std::chrono;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initViewers();
    void showFrames();
    void updateData();

    fullScreenVideoViewer *fsVidWidget;
    VideoWidget *vidWid;

private slots:
    void callStartStream();
    void callStopStream();
    void callSetStreamSource(QString);
    void callSetVideoName(QString source);

private:
    QWidget *cen;
    Mat globalFrame;
    Mat defaultFrame;
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *ifStream;
    QDBusConnection bus2 = QDBusConnection::sessionBus();
    QDBusInterface *ifMaster;
    QDBusConnection bus3 = QDBusConnection::sessionBus();
    QDBusInterface *ifTrack;
    QDBusConnection bus4 = QDBusConnection::sessionBus();
    QDBusInterface *ifIPM;
    QSharedMemory sm;
    struct sharedData {
        uchar rawImg[FRAME_SIZE];
    };
    QSharedMemory sm2;
    struct sharedData2 {
        int actualLeftSize;
        int actualRightSize;
        float leftPts[PTS_NUM][2];
        float rightPts[PTS_NUM][2];
    };
    QSharedMemory sm3;
    struct sharedData3 {
        int leftStatus[2];
        int rightStatus[2];
    };
    vector<Vec2i> bottomPts;
    int frameCount;
    vector<Vec2i> topPts;
    vector<Vec2i> leftPts;
    vector<Vec2i> rightPts;
    vector<int> colorRange;
    vector<vector<Point>> prevContours;
    bool updateDataLock;
    Logger log;
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    Scalar leftColor;
    Scalar rightColor;

    void drawFinalRGB();
};

#endif // MAINWINDOW_H
