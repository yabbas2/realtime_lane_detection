#ifndef STREAM_HPP
#define STREAM_HPP

#include <opencv2/opencv.hpp>
#include <thread>
#include <iostream>
#include <mutex>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class Stream
{
    private:
        static Mat frame;
        static VideoCapture cap;
        static double fps;
        static const char endStream;
        static bool stopStream;
        //static mutex lock;
    public:
        static bool setVideoSource(string source);
        static void videoInStream();
        static Mat readFrame();
        static void videoOutStream();
};

#endif /* STREAM_HPP */

