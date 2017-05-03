#include "processing.hpp"

using namespace cv;
using namespace std;

bool processing::waitFlag = false;
Mat processing::pts;
Mat processing::dst;
double processing::afps = 0;

void processing::initIPM()
{
    while(normalFrame.empty());
    int height = normalFrame.rows;
    int width = normalFrame.cols;
    dst = (Mat_<double>(4,2) << 90, 0, height-90, 0, height-90, width, 90, width);
    switch(videoSourceNum) {
        case '1':
            pts = (Mat_<double>(4, 2) << 253, 219, 352, 219, 444, 288, 60, 288); break;
        case '2':
            pts = (Mat_<double>(4, 2) << 197, 212, 326, 212, 348, 230, 106, 230); break;
        case '3':
            pts = (Mat_<double>(4, 2) << 220, 200, 322, 200, 361, 230, 85, 230); break;
        case '4':
            pts = (Mat_<double>(4, 2) << 230, 211, 342, 211, 373, 238, 160, 238); break;
        case '5':
            pts = (Mat_<double>(4, 2) << 241, 208, 345, 208, 450, 297, 23, 297); break;
        default: return;
    }
    pts.convertTo(pts, CV_32F);
    dst.convertTo(dst, CV_32F);
}

void processing::laneDetection()
{
    initIPM();
    while(true) {
        while (waitFlag);

        if (normalFrame.empty())
            return;

        double s1 = getTickCount();
        //determineROI();
        fourPointTransform();
        double d1 = (double(getTickCount()) - s1) * 1000 / getTickFrequency();

        double s2 = getTickCount();
        LSD();
        double d2 = (double(getTickCount()) - s2) * 1000 / getTickFrequency();

        double s3 = getTickCount();
        inverse();
        double d3 = (double(getTickCount()) - s3) * 1000 / getTickFrequency();

        //cout << "IPM_1: " << d1 << "\t\t" << "LSD: " << d2 << "\t\t" << "IPM_2: " << d3 << "\t\t" << "Total: " << d1+d2+d3 << endl;
        afps = 1000 / (d1+d2+d3);
        waitFlag = true;
    }
}