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
    KalmanFilter leftKalman [20];
    KalmanFilter rightKalman [20];

public:
    Kalman();

};

#endif // KALMAN_H
