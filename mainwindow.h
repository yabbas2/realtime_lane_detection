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
#include <QGraphicsOpacityEffect>
#include <QList>
#include <QPropertyAnimation>
#include "front-end/sidebar/side_bar.h"
#include "front-end/viewers/multivideoviewer.h"
#include "front-end/viewers/fullscreenvideoviewer.h"

#define mainWindowWidth     854
#define mainWindowHeight    480
#define sideBarWidth        300
#define multiVideoWidth     (mainWindowWidth-sideBarWidth)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MultiVideoViewer *getMultiVideoViewerWidget();
    fullScreenVideoViewer *getFullScreenVideoViewerWidget();

private slots:
    void switchToFullScreen();

private:
    MultiVideoViewer *mulVidWidget;
    fullScreenVideoViewer *fsVidWidget;
    Side_bar *sidebar;
    QWidget *cen1;
    QWidget *cen2;
    QGridLayout *grid_main_layout;
    QPropertyAnimation* animation;
    QPropertyAnimation* animation2;
    QGraphicsOpacityEffect *mEffect;
    QGraphicsOpacityEffect *wEffect;

signals:
    void sidebar_disappear();

private slots:
    void ToFullScreenAnimationFinish();
};

#endif // MAINWINDOW_H
