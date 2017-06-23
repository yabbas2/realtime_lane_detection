#ifndef IPM_H
#define IPM_H

#include <QString>
#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace ipm {
enum {left, right};
}

class IPM
{
private:
    Mat ipm_frame;
    Mat inverse_homography;
    vector<Vec2f> leftPoints;
    vector<Vec2f> rightPoints;
public:
    void transform(Mat &original_frame, QString vido_name);
    void inverseTransform(vector<Vec2f> &pts, char c);
    Mat *getIPMFrame();
    vector<Vec2f>* getPoints(int side);
};

#endif // IPM_H
