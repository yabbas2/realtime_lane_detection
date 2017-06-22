#ifndef GUI_H
#define GUI_H

#include <QApplication>
#include <QtDBus>
#include "front-end/mainwindow.h"

class GUI : public QApplication
{
    Q_OBJECT

public:
    explicit GUI(int argc, char *argv[]);
    MainWindow mainWindow;
};

#endif // GUI_H
