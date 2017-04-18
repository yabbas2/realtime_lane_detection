#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <QListView>
#include <QStringListModel>
#include <QComboBox>
#include <QSlider>
#include <QGridLayout>
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

private:
    MultiVideoViewer *mulVidWidget;
    fullScreenVideoViewer *fsVidWidget;
    Side_bar *sidebar;
    QWidget *cen;
    QGridLayout *grid_main_layout;
};

#endif // MAINWINDOW_H
