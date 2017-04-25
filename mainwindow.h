#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <QListView>
#include <QStringListModel>
#include <QComboBox>
#include <QSlider>
#include <QGridLayout>
#include <QList>
#include <QStackedWidget>
#include "front-end/sidebar/side_bar.h"
#include "front-end/viewers/multivideoviewer.h"
#include "front-end/viewers/fullscreenvideoviewer.h"
#include "front-end/videowidget.h"

#define mainWindowWidth     854
#define mainWindowHeight    480
#define sideBarWidth        300
#define sideBarHeight       (mainWindowHeight-50)
#define multiVideoWidth     (mainWindowWidth-sideBarWidth)
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
    Side_bar *getSideBarWidget();

private slots:
    void switchToFullScreen();
    void switchToMainScreen();

private:
    MultiVideoViewer *mulVidWidget;
    fullScreenVideoViewer *fsVidWidget;
    Side_bar *sidebar;
    VideoWidget *vidWid;
    QWidget *cen1;
    QWidget *cen2;
    QGridLayout *grid_main_layout;
    QStackedWidget *widgetStack;

};

#endif // MAINWINDOW_H
