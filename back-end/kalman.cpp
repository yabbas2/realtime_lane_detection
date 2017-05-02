#include "kalman.h"

Kalman::Kalman()
{
    for (int i = 0; i < 20; ++i) {
        leftKalman[i] = KalmanFilter(4, 2, 0, CV_64F);
        leftKalman[i].measurementMatrix = (Mat_ <double>(2,4)<< 1, 0, 0, 0, 0, 1, 0, 0);
        leftKalman[i].transitionMatrix = (Mat_ <double>(4,4)<< 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
        leftKalman[i].processNoiseCov = (Mat_ <double>(4,4)<< 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) * 0.0003;
        rightKalman[i] = KalmanFilter(4, 2, 0, CV_64F);
        rightKalman[i].measurementMatrix = (Mat_ <double>(2,4)<< 1, 0, 0, 0, 0, 1, 0, 0);
        rightKalman[i].transitionMatrix = (Mat_ <double>(4,4)<< 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
        rightKalman[i].processNoiseCov = (Mat_ <double>(4,4)<< 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) * 0.0003;
    }
    isMeasure = true;
}

void Kalman::kalmanFilter(vector<Vec2i> &points, char c)
{
    if (c == kalman::left_region)
        newLeftPoints = points;
    else
        newRightPoints = points;
    smoothing(c);
}

void Kalman::smoothing(char &c)
{
    KalmanFilter* k;
    vector<Vec2i>* prevPoints;
    vector<Vec2i>* newPoints;
    if (c == kalman::left_region)
    {
        prevPoints = &prevLeftPoints;
        newPoints = &newLeftPoints;
        k = leftKalman;
    }
    else if (c == kalman::right_region)
    {
        prevPoints = &prevRightPoints;
        newPoints = &newRightPoints;
        k = rightKalman;
    }
    Mat tp;
    Mat mp = Mat::zeros(2, 1, CV_64F);
    if(isMeasure)
    {
        isMeasure = false;
        for (int i = 0; i < 20; ++i) {
            mp.at<double>(0, 0) = newPoints->at(i)[0];
            mp.at<double>(1, 0) = newPoints->at(i)[1];
            for (int j = 0; j < 50; ++j) {
                k[i].correct(mp);
                tp = k[i].predict();
            }
        }
    }
    for (int i = 0; i < 20; ++i) {
        mp.at<double>(0, 0) = newPoints->at(i)[0];
        mp.at<double>(1, 0) = newPoints->at(i)[1];
        k[i].correct(mp);
        tp = k[i].predict();
        prevPoints->push_back(Vec2i{(int)tp.at<double>(0, 0), (int)tp.at<double>(1, 0)});
    }
}
