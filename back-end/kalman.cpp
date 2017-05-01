#include "kalman.h"

Kalman::Kalman()
{
    KalmanFilter KF(4,2);
    KF.measurementMatrix = (Mat <int>(4,2)<< 1, 0, 0, 0, 0, 1, 0, 0);
    KF.transitionMatrix = (Mat <int>(4,4)<< 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
    KF.processNoiseCov = (Mat <int>(4,4)<< 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) * 0.0003;
    for (int i = 0; i < 20; ++i) {
        leftKalman[i] = KF;
        rightKalman[i] = KF;
    }
}

void Kalman::smoothing(char c)
{
    vector<KalmanFilter>* k;
    vector<Vec2i>* points;
    if (c == 'l')
    {
        k = &leftKalman;
        points = &newLeftPoints;
    }
    else
    {
        k = &rightKalman;
        points = &newRightPoints;
    }
    if((*k)[0].statePre == 0)
        for (int i = 0; i < 20; ++i) {
            Mat mp = (Mat <int>(2,1)<< (*points)[i]);
            for (int j = 0; j < 50; ++j) {

            }
        }
    leftKalman.
}
