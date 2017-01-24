#ifndef PROCESSING_HPP
#define PROCESSING_HPP

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
        static void preHT();
        static void preLSD();
        static void LSD();
        static void filterAndTakeAverage(int start, int end, unsigned int windowSize, unsigned int threshold);
        static void orderPoints();
        static void fourPointTransform();
        static void inverse();
        static void houghTransform();
        static void takeAverage(int start, int end, unsigned int windowSize, unsigned int threshold);
        static void checkForAllLanes();
    public:
        static void laneDetection();
        static void setFrame(Mat f);
        static list<Vec4d> * getLanesPositions();
};

#endif /* PROCESSING_HPP */

