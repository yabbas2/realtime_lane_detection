#include "kalman.h"

Kalman::Kalman()
{
    KalmanFilter KF(4,2);
    KF.measurementMatrix = (Mat_ <int>(4,2)<< 1, 0, 0, 0, 0, 1, 0, 0);
    KF.transitionMatrix = (Mat_ <int>(4,4)<< 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
    KF.processNoiseCov = (Mat_ <int>(4,4)<< 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) * 0.0003;
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
        c = 'r';
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
    if(k->statePre.empty())
        for (int i = 0; i < 20; ++i) {
            mp = (Mat_ <int>(1,2)<< newPoints->at(i)[0], newPoints->at(i)[1]);
            for (int j = 0; j < 50; ++j) {
                (*(k+i)).correct(mp);
                tp = (*(k+i)).predict();
            }
        }
    for (int i = 0; i < 20; ++i) {
        mp = (Mat_ <int>(1,2)<< newPoints->at(i)[0], newPoints->at(i)[1]);
        (*(k+i)).correct(mp);
        tp = (*(k+i)).predict();
        prevPoints->push_back(tp);
    }
}
