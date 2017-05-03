#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <list>
#include <iostream>

#define show_ipm                    0
#define show_final                  1
#define writeVideo                  0

using namespace cv;
using namespace std;

class processing
{
    private:
        static Mat normalFrame;
        static Mat ipmFrame;
        static Mat pts;
        static Mat dst;
        static Mat roiImg;
        static bool waitFlag;
        static int marginCount;
        static string lanesNumber;
        static int lineMargin;
        static vector<int> arrow;
        static Mat HomographyToOriginal;
        static vector<Vec4f> lines;
        static list<Vec4f> detectedLanes;
        static VideoCapture cap;
        static Mat frameToShow;
        static char videoSourceNum;
        static double fps;
        static double afps;
        //static double framesNumber;
        static const char endStream;
        //static double error[3];
        //static double accuracy;
        //static unsigned int mismatch;
        static void LSD();
        static void filterAndTakeAverage(int start, int end, unsigned int windowSize, int margin);
        static void fourPointTransform();
        static void inverse();
        static void initIPM();
        static void draw();
        static int checkLanesFromLeft();
        static int checkLanesFromRight();
        static void determineROI();
        //static void calAccuracy();
        //static void printAccuracy();
    public:
        static void laneDetection();
        static bool setVideoSource(string source);
        static void videoIOStream();
};

#endif /* PROCESSING_HPP */

