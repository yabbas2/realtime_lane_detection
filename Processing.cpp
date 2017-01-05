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
vector<Vec4d> Processing::getLanesPositions()
{
    return vector<Vec4d>(detectedLanes.begin(), detectedLanes.end());
}