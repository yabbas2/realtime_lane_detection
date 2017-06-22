#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QtDBus>
#include <opencv2/core.hpp>
#include "sidebar/side_bar.h"
#include "viewers/multivideoviewer.h"
#include "viewers/fullscreenvideoviewer.h"
#include "videowidget.h"

#define mainWindowWidth     800
#define mainWindowHeight    480
#define sideBarWidth        300
#define sideBarHeight       (mainWindowHeight-50)
#define multiVideoHeight    (mainWindowHeight-50)
#define multiVideoWidth     (mainWindowWidth-sideBarWidth-30)
#define videoWidgetWidth    300

using namespace cv;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initViewers();
    MultiVideoViewer *mulVidWidget;
    fullScreenVideoViewer *fsVidWidget;
    SideBar *sidebar;
    VideoWidget *vidWid;

private slots:
    void switchToFullScreen(int index);
    void switchToMainScreen();

private:
    QWidget *cen1;
    QWidget *cen2;
    QHBoxLayout *h_cen1_layout;
    QStackedWidget *widgetStack;
    Mat normal_default_screen;
    Mat ipm_default_screen;
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *ifStream;
};

#endif // MAINWINDOW_H
