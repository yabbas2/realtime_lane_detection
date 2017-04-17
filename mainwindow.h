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
#include "front-end/multivideoviewer.h"
#include <QListView>
#include <QStringListModel>
#include <QComboBox>
#include <QSlider>
#include <QGridLayout>
#include "front-end/side_bar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MultiVideoViewer *getMultiVideoViewerWidget();

private:
    MultiVideoViewer *mulVidWidget;
    Side_bar *sidebar;
    QWidget *cen;
    QGridLayout *grid_main_layout;
};

#endif // MAINWINDOW_H
