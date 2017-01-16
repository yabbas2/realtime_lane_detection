#include "Processing.hpp"

Mat Processing::normalFrame;
Mat Processing::ipmFrame;

void Processing::setFrame(Mat f)
{
    normalFrame = f;
    ipmFrame = f;
}
void Processing::laneDetection()
{
    
}
list<Vec4d> * Processing::getLanesPositions()
{
    return &detectedLanes;
}