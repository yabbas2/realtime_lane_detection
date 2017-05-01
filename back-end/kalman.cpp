#include "kalman.h"

Kalman::Kalman()
{
    KalmanFilter KF(4,2);
    KF.measurementMatrix = (Mat_ <int>(2,4)<< 1, 0, 0, 0, 0, 1, 0, 0);
    KF.transitionMatrix = (Mat_ <int>(4,4)<< 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
    KF.processNoiseCov = (Mat_ <int>(4,4)<< 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) * 0.0003;
    for (int i = 0; i < 20; ++i) {
        leftKalman[i] = KF;
        rightKalman[i] = KF;
    }
    isMeasure = true;
}

void Kalman::kalmanFilter(vector<Vec2i> &points, char c)
{
    if (c == 'l')
        newLeftPoints = points;
    else
        newRightPoints = points;
    smoothing(c);
}

void Kalman::smoothing(char &c)
{
    KalmanFilter* k;
    vector<Vec2i>* newPoints;
    vector<Vec2i>* prevPoints;
    if (c == 'l')
    {
        prevPoints = &prevLeftPoints;
        newPoints = &newLeftPoints;
        k = leftKalman;
    }
    else
    {
        prevPoints = &prevRightPoints;
        newPoints = &newRightPoints;
        k = rightKalman;
    }
    Mat mp, tp;
    if(isMeasure)
    {
        isMeasure = false;
        for (int i = 0; i < 20; ++i) {
            mp = (Mat_ <int>(2,1)<< newPoints->at(i)[0], newPoints->at(i)[1]);
            mp.convertTo(mp, CV_64F);
            for (int j = 0; j < 50; ++j) {
                k[i].correct(mp);
                tp = k[i].predict();
            }
        }
    }
    for (int i = 0; i < 20; ++i) {
        mp = (Mat_ <int>(2,1)<< newPoints->at(i)[0], newPoints->at(i)[1]);
        k[i].correct(mp);
        tp = k[i].predict();
        prevPoints->push_back(tp);
    }

}
