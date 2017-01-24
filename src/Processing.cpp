#include "Processing.hpp"


void Processing::laneDetection()
{
    if(normalFrame.empty())
        return;
    double s1 = getTickCount();
    fourPointTransform();
    double d1 = (double(getTickCount()) - s1) * 1000 / getTickFrequency();
    double s2 = getTickCount();
    preLSD();
    LSD();
    double d2 = (double(getTickCount()) - s2) * 1000 / getTickFrequency();
    double s3 = getTickCount();
    //preHT();
    //houghTransform();
    double d3 = (double(getTickCount()) - s3) * 1000 / getTickFrequency();
    double s4 = getTickCount();
    inverse();
    double d4 = (double(getTickCount()) - s4) * 1000 / getTickFrequency();
    cout << d1 << "\t" << d2 << "\t" << d3 << "\t" << d4 << endl;
}