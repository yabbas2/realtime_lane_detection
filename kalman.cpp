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
    isMeasure = 0;
}

void Kalman::kalmanFilter(vector<Vec2f> &points, char c)
{
    if(points.empty())
    {
        if (c == kalman::left)
            newLeftPoints = prevLeftPoints;
        else if(c == kalman::right)
            newRightPoints = prevRightPoints;
        count++;
    }
    else
    {
        if (c == kalman::left)
            newLeftPoints = points;
        else if(c == kalman::right)
            newRightPoints = points;
        count = 0;
    }
    smoothing(c);
}

void Kalman::smoothing(char &c)
{
    KalmanFilter* k;
    vector<Vec2f>* prevPoints;
    vector<Vec2f>* newPoints;
    if (c == kalman::left)
    {
        prevPoints = &prevLeftPoints;
        newPoints = &newLeftPoints;
        k = leftKalman;
    }
    else if (c == kalman::right)
    {
        prevPoints = &prevRightPoints;
        newPoints = &newRightPoints;
        k = rightKalman;
    }
    prevPoints->clear();
    if (newPoints->empty())
        return;
    Mat tp;
    Mat mp = Mat::zeros(2, 1, CV_64F);
    if(isMeasure < 2)
    {
        isMeasure++;
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
        prevPoints->push_back(Vec2f{(float)tp.at<double>(0, 0), (float)tp.at<double>(1, 0)});
    }
}

vector<Vec2f> *Kalman::getPrevPoints(int side)
{
    switch(side)
    {
        case kalman::left:
        return &prevLeftPoints;
        case kalman::right:
        return &prevRightPoints;
    }
}

int *Kalman::getCount(){return &count;}
