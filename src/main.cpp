#include <cstdlib>
#include <thread>
//#include <opencv2/opencv.hpp>
#include "Stream.hpp"
#include "Processing.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    bool streamFlag;
    streamFlag = Stream::setVideoSource(argv[1]);
    assert(streamFlag);
    thread video(Stream::videoIOStream);
    video.detach();
    assert(!video.joinable());
    while(true)
    {
        
    }
    return 0;
}

