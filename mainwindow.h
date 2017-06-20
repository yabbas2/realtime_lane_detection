#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <QStackedWidget>
#include <QHBoxLayout>
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MultiVideoViewer *getMultiVideoViewerWidget();
    fullScreenVideoViewer *getFullScreenVideoViewerWidget();
    VideoWidget *getVideoWidget();
    SideBar *getSideBarWidget();

private slots:
    void switchToFullScreen(int index);
    void switchToMainScreen();

private:
    MultiVideoViewer *mulVidWidget;
    fullScreenVideoViewer *fsVidWidget;
    SideBar *sidebar;
    VideoWidget *vidWid;
    QWidget *cen1;
    QWidget *cen2;
    QHBoxLayout *h_cen1_layout;
    QStackedWidget *widgetStack;
};

#endif // MAINWINDOW_H
