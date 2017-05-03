#ifndef IPM_H
#define IPM_H

#include <QString>
#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class IPM
{
private:
    Mat ipm_frame;
    Mat inverse_homography;

public:
    void transform(Mat &original_frame, QString vido_name);
    void inverseTransform(vector<Vec2f> &pts);
    Mat *getIPMFrame();

};

#endif // IPM_H
