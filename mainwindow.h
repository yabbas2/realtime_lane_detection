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
#include "side_bar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Side_bar *sidebar;
};

#endif // MAINWINDOW_H
