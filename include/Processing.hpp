#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#define _USE_MATH_DEFINES

#include <opencv2/opencv.hpp>
#include <vector>
#include <list>
#include <opencv2/core/matx.hpp>
#include <math.h>

using namespace cv;
using namespace std;

class Processing
{
    private:
        static Mat normalFrame;
        static Mat ipmFrame;
        static vector<Vec4d> lines;
        static list<Vec4d> detectedLanes;
        static void buildGaborFilter();
        static void gaborProcess();
        static void gtnProcess();
        static void orderPoints();
        static void fourPointTransform();
        static void inverse();
        static void houghTransform();
        static void takeAverage(unsigned int start, unsigned int end, unsigned int windowSize, unsigned int threshold);
        static void checkForAllLanes();
    public:
        static void laneDetection();
        static void setFrame(Mat f);
        static vector<Vec4d> getLanesPositions();
};

#endif /* PROCESSING_HPP */

