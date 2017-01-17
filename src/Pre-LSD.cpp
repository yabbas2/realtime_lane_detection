#include <opencv2/opencv.hpp>
#include "Processing.hpp"

using namespace cv;

void Processing::preLSD()
{
    cvtColor(ipmFrame, ipmFrame, COLOR_BGR2GRAY);
    //OutputArray mu();
    //OutputArray sigma();
    //meanStdDev(ipmFrame, mu(), sigma());
    Canny(ipmFrame, ipmFrame, 50, 200, 3, true);
}