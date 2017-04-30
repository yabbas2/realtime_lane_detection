#ifndef FILTER_H
#define FILTER_H

#include<opencv2/opencv.hpp>
#include <vector>

#define threshold_angle_min     70
#define threshold_angle_max     110
#define threshold_length        30.0

using namespace std;
using namespace cv;

class Filter
{
public:
    Filter();
    vector<Vec4i> * getFilteredLines();
    void falseDetectionElimination(Mat &ipm_frame, vector<Vec4i> &l);

private:
    vector<Vec4i> lines;
};

#endif // FILTER_H
