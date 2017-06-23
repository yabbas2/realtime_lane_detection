#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <QtDBus>
#include <QSharedMemory>
#include <opencv2/core.hpp>
#include "viewers/fullscreenvideoviewer.h"
#include "videowidget.h"

#define mainWindowWidth     800
#define mainWindowHeight    480
#define videoWidgetWidth    300
#define FRAME_SIZE          1152000
#define frameHeight         480
#define frameWidth          800

using namespace cv;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initViewers();
    void showFrames();
    void setSharedKey(QString keyId);
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
    struct sharedData {
        uchar rawImg[FRAME_SIZE];
    };
    QSharedMemory sm;
};

#endif // MAINWINDOW_H
