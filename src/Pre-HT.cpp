#include "Processing.hpp"

using namespace cv;

void Processing::preHT()
{
    cvtColor(ipmFrame, ipmFrame, COLOR_BGR2GRAY);
    GaussianBlur(ipmFrame, ipmFrame, Size(11, 11), 0);
    adaptiveThreshold(ipmFrame, ipmFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 5, 4);
    Canny(ipmFrame, ipmFrame, 50, 200, 3, true);
}