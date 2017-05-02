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
    vector<Vec2i> prevLeftPoints;
    vector<Vec2i> prevRightPoints;
    vector<Vec2i> newLeftPoints;
    vector<Vec2i> newRightPoints;
    bool isMeasure;
    void smoothing(char &c);
    void leftSmoothing();
    void rightSmoothing();

public:
    Kalman();
    void kalmanFilter(vector<Vec2i> &points, char c);
    vector<Vec2i>* getPrevLeftPoints();
    vector<Vec2i>* getPrevRightPoints();
    int* getCount();
};

#endif // KALMAN_H
