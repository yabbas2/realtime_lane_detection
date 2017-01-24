#include "Processing.hpp"

using namespace cv;
Mat Processing::gaborFilter[3];
void Processing::buildGaborFilter(){
    double ksize = 31;
    // kernel = getGaborKernel(Size(kernel_size,kernel_size), sig, th, lm, gm, ps);
    int theta [3] = {359, 0, 1};
    for (int i = 0; i < 3 ; ++i) {
        gaborFilter[i] = getGaborKernel(Size(ksize,ksize), 4, theta[i], 10,0, 0);
    }
    //gaborFilter /= 1.5 * sum(gaborFilter)[0];
}


void Processing::gaborProcess(){
    Mat maxFrame = ipmFrame.clone();
    Mat tmp = ipmFrame.clone();
    for (int i = 0; i < 3 ; ++i) {
        filter2D(ipmFrame, tmp, CV_32F, gaborFilter[i]);
        maxFrame = max(maxFrame, tmp);
    }
    ipmFrame = maxFrame.clone();
}