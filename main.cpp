#include <cstdlib>
#include <thread>
#include <opencv2/opencv.hpp>
#include "Stream.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    bool streamFlag;
    streamFlag = Stream::setVideoSource(argv[1]);
    assert(streamFlag);
    thread videoIn(Stream::videoInStream);
    thread videoOut(Stream::videoOutStream);
    while(true);
    return 0;
}

