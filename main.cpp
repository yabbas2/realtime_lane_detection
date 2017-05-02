#include <QApplication>
#include "mainwindow.h"
#include "back-end/pipeline.h"
#include "back-end/kalman.h"
#include "back-end/filter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Pipeline p;
    p.connectFrontEndToBackEnd(&w);


    return a.exec();
}
