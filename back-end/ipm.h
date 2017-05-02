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
    vector<Vec2i> new_pts;

public:
    void transform(Mat &original_frame, QString vido_name);
    void inverseTransform(vector<Vec2i> &old_pts);
    Mat *getIPMFrame();
    vector<Vec2i> *getFinalPoints();

};

#endif // IPM_H
