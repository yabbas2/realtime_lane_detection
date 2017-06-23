#ifndef IPM_H
#define IPM_H

#include <QString>
#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/photo.hpp>

#define frameHeight         480
#define frameWidth          800

using namespace cv;
using namespace std;

namespace ipm {
enum {left, right};
}

class IPM
{
public:
    IPM();
    void transform(Mat &frame);
    void setVideoPts(QString);
    Mat *getIPMFrame();
    Mat ipmFrame;

private:
    Mat inverseHomography;
    Mat transformHomography;
    Mat dstPts;
    Mat inputPts;
};

#endif // IPM_H
