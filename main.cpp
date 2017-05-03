#include <QApplication>
#include "mainwindow.h"
#include "back-end/pipeline.h"
#include "back-end/stream.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Stream st;

    Pipeline p;
    p.connectToStreamEngine(&st);
    p.connectFrontEndToBackEnd(&w);


    return a.exec();
}
