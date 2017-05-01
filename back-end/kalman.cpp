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

void Kalman::kalmanFilter(vector<Vec2i> &points, int &width)
{
    char c;
    if (points[0][0] < width/2)
    {
        c = 'l';
        newLeftPoints = points;
    }
    else
    {
        c = "r";
        newRightPoints = points;
    }
    smoothing(c);
}

void Kalman::smoothing(char &c)
{
    KalmanFilter* k;
    vector<Vec2i>* newPoints;
    vector<Vec2i>* prevPoints;
    if (c == 'l')
    {
        newPoints = &newLeftPoints;
        k = &leftKalman;
    }
    else
    {
        newPoints = &newRightPoints;
        k = &rightKalman;
    }
    Mat mp, tp;
    if((*k).statePre[0][0] == 0)
        for (int i = 0; i < 20; ++i) {
            mp = (Mat <int>(2,1)<< (*newPoints)[i]);
            for (int j = 0; j < 50; ++j) {
                (*(k+i)).correct(mp);
                tp = (*(k+i)).predict();
            }
        }
    for (int i = 0; i < 20; ++i) {
        mp = (Mat <int>(2,1)<< (*newPoints)[i]);
        (*(k+i)).correct(mp);
        tp = (*(k+i)).predict();
        prevPoints->push_back(tp);
    }
}
