#ifndef KALMAN_H
#define KALMAN_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "opencv2/video/tracking.hpp"

using namespace std;
using namespace cv;

class Kalman
{
private:
    vector<KalmanFilter> leftKalman[20];
    vector<KalmanFilter> rightKalman[20];
    vector<Vec2i> prevLeftPoints[20];
    vector<Vec2i> prevRightPoints[20];
    vector<Vec2i> newLeftPoints[20];
    vector<Vec2i> newRightPoints[20];
public:
    Kalman();
    void smoothing(char c);
};

#endif // KALMAN_H
