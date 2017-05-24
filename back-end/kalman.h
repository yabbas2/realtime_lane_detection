#ifndef KALMAN_H
#define KALMAN_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "opencv2/video/tracking.hpp"

using namespace std;
using namespace cv;

typedef Vec<int, 7> Vec7i;

namespace kalman {
enum {left_region, right_region};
}

class Kalman
{
private:
    int count = 0;
    KalmanFilter leftKalman[20];
    KalmanFilter rightKalman[20];
    vector<Vec2f> prevLeftPoints;
    vector<Vec2f> prevRightPoints;
    vector<Vec2f> newLeftPoints;
    vector<Vec2f> newRightPoints;
    int isMeasure;
    void smoothing(char &c);
    void leftSmoothing();
    void rightSmoothing();

public:
    Kalman();
    void kalmanFilter(vector<Vec2f> &points, char c);
    vector<Vec2f>* getPrevPoints(int side);
    int* getCount();
};

#endif // KALMAN_H
