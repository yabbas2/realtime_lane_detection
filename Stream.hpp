#ifndef STREAM_HPP
#define STREAM_HPP

#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core/matx.hpp>

using namespace std;
using namespace cv;

class Stream
{
    private:
        static Mat frame;
        static vector<Vec4d> lanes;
        static VideoCapture cap;
        static double fps;
        static double framesNumber;
        static const char endStream;
    public:
        static bool setVideoSource(string source);
        static void videoIOStream();
        static Mat readFrame();
        static double getFrameCount();
        static void setLanesPositions(vector<Vec4d> lines);
};

#endif /* STREAM_HPP */

