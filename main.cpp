#include "videostatistics.h"
#include "videocontrols.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VideoStatistics vs;
    vs.show();
    VideoControls vc;
    vc.show();
    return a.exec();
}
