#include <cstdlib>
#include <thread>
#include "processing.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    bool streamFlag;
    streamFlag = processing::setVideoSource(argv[1]);
    assert(streamFlag);
    thread video(processing::videoIOStream);
    video.detach();
    assert(!video.joinable());
    processing::laneDetection();
}

