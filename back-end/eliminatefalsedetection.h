#ifndef ELIMINATEFALSEDETECTION_H
#define ELIMINATEFALSEDETECTION_H

#define threshold_angle_min 70
#define threshold_angle_max 110
#define threshold_length 30.0

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "math.h"

using namespace std;
using namespace cv;

class eliminateFalseDetection
{
public:
    vector<vector<int> > getLines();
    void falseElimination(Mat hsv_frame, Mat ipm_frame, vector<vector<float>> &l);
private:
    vector<vector<int>> lines;

};

#endif // ELIMINATEFALSEDETECTION_H
