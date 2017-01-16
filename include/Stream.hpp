#ifndef STREAM_HPP
#define STREAM_HPP

#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core/matx.hpp>
#include <list>

using namespace std;
using namespace cv;

class Stream
{
    private:
        static Mat frame;
        static list<Vec4d> lanes;
        static VideoCapture cap;
        static double fps;
        static double framesNumber;
        static const char endStream;
    public:
        static bool setVideoSource(string source);
        static void videoIOStream();
        static Mat readFrame();
        static double getFrameCount();
        static void setLanesPositions(list<Vec4d> * lines);
};

#endif /* STREAM_HPP */

