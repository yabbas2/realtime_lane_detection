#include <cstdlib>
#include <thread>
#include "Processing.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    bool streamFlag;
    streamFlag = Processing::setVideoSource(argv[1]);
    assert(streamFlag);
    thread video(Processing::videoIOStream);
    video.detach();
    assert(!video.joinable());
    Processing::laneDetection();
}

