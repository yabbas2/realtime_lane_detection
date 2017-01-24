#include "Processing.hpp"

using namespace cv;

Mat Processing::gaborFilter;

void Processing::buildGaborFilter(){
    int ksize = 31;
    gaborFilter = getGaborKernel(Size(ksize,ksize), 4, 0, 10, 0, 0);
}

void Processing::gaborProcess(){
    ipmFrame.convertTo(ipmFrame, CV_32F);
    filter2D(ipmFrame, ipmFrame, CV_32F, gaborFilter);
    ipmFrame.convertTo(ipmFrame,CV_8U, 10.0/255.0);
}
