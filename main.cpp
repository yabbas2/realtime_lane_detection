#include "mainwindow.h"
#include <QApplication>
#include "back-end/stream_in.h"
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    StreamIn *stream_in = new StreamIn;
    QThread streamInThread;
    stream_in->initStreamIn("/dev/video0", streamInThread);
    stream_in->moveToThread(&streamInThread);
    streamInThread.start();

    return a.exec();
}
