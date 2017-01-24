#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <list>
#include <math.h>
#include <opencv2/core/matx.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Processing
{
    private:
        static Mat normalFrame;
        static Mat ipmFrame;
        static Mat pts;
        static Mat dst;
        static Mat gaborFilter;
        static Mat HomographyToOriginal;
        static vector<Vec4d> lines;
        static list<Vec4d> detectedLanes;
        static VideoCapture cap;
        static double fps;
        static double framesNumber;
        static const char endStream;
        static void buildGaborFilter();
        static void gaborProcess();
        static void preHT();
        static void preLSD();
        static void LSD();
        static void filterAndTakeAverage(int start, int end, unsigned int windowSize, unsigned int threshold);
        static void fourPointTransform();
        static void inverse();
        static void houghTransform();
        static void takeAverage(int start, int end, unsigned int windowSize, unsigned int threshold);
        static void checkForAllLanes();
    public:
        static void laneDetection();
        static bool setVideoSource(string source);
        static void videoIOStream();
        static double getFrameCount();
};

#endif /* PROCESSING_HPP */

