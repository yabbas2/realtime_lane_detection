#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <list>
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
        static bool waitFlag;
        static Mat HomographyToOriginal;
        static vector<Vec4f> lines;
        static list<Vec4f> detectedLanes;
        static VideoCapture cap;
        static char videoSourceNum;
        static double fps;
        //static double framesNumber;
        static const char endStream;
        static void LSD();
        static void filterAndTakeAverage(int start, int end, unsigned int windowSize, unsigned int threshold);
        static void fourPointTransform();
        static void inverse();
        static void initIPM();
    public:
        static void laneDetection();
        static bool setVideoSource(string source);
        static void videoIOStream();
};

#endif /* PROCESSING_HPP */

