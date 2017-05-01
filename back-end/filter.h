#ifndef FILTER_H
#define FILTER_H

#include <opencv2/opencv.hpp>
#include <vector>

#define threshold_angle_min     70
#define threshold_angle_max     110
#define threshold_length        30.0

using namespace std;
using namespace cv;

typedef Vec<int, 7> Vec7i;

class Filter
{
public:
    Filter();
    vector<Vec7i>* getFilteredLines();
    void falseDetectionElimination(Mat &ipm_frame, vector<Vec4i> &l);

private:
    vector<Vec7i> lines;
};

#endif // FILTER_H
