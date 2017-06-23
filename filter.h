#ifndef FILTER_H
#define FILTER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <QDebug>

#define threshold_angle_min     70
#define threshold_angle_max     110
#define threshold_length        50

using namespace std;
using namespace cv;

typedef Vec<int, 7> Vec7i;

class Filter
{
public:
    Filter();
    vector<Vec7i>* getFilteredLines();
    Mat falseDetectionElimination(Mat &ipm_frame, vector<Vec4f> &l);

private:
    vector<Vec7i> lines;
    Mat hsv_frame;
};

#endif // FILTER_H
