#ifndef TRACK_H
#define TRACK_H

#include <opencv2/core.hpp>
#include <vector>
#include <QApplication>
#include <QSharedMemory>
#include <QtDBus>
#include <chrono>
#include "../logger/logger.h"

using namespace std;
using namespace cv;
using namespace std::chrono;

class Track : public QApplication
{
    Q_OBJECT

public:
    explicit Track(int &argc, char **argv);
    void process();

    bool busy;

private:
    Logger log;
    int frameCount;
};

#endif // TRACK_H
