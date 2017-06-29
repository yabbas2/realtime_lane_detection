#include "track.h"

Track::Track(int &argc, char **argv) :
    QApplication(argc, argv)
{
    log.openFile(qApp->applicationDirPath() + "/../logger/logFiles/log_track.txt", QIODevice::WriteOnly);
    log.write("----------------------------------------------------------");
    log.write("------------------------NEW RUN---------------------------");
    log.write("----------------------------------------------------------");
    frameCount = 0;
    busy = false;
}
