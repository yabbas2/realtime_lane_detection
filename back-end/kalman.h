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
    KalmanFilter leftKalman[20];
    KalmanFilter rightKalman[20];
    vector<Vec2i> prevLeftPoints;
    vector<Vec2i> prevRightPoints;
    vector<Vec2i> newLeftPoints;
    vector<Vec2i> newRightPoints;
public:
    Kalman();
    void kalmanFilter(vector<Vec2i> &points, int &width);
    void smoothing(char &c);
    void leftSmoothing();
    void rightSmoothing();
};

#endif // KALMAN_H
