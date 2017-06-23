#include <QCoreApplication>
#include "ipm.h"
#include <QDebug>
#include <chrono>

using namespace cv;
using namespace chrono;

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    IPM ipmObj;
    string filename = "/home/yousef/projects/real-time_lane_detection/Data/samplesVideos/youtube_video1.mp4";
    VideoCapture capture(filename);
    capture.open(filename);
    Mat normalFrame;
    Mat ipmFrame;
    if( !capture.isOpened() )
        throw "Error when reading steam_avi";

    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    int counter = 0;
    long allTime = 0;
    while(true)
    {
        t1 = high_resolution_clock::now();
        capture >> normalFrame;
        if(normalFrame.empty())
            break;
        ipmObj.transform(normalFrame, "youtube_video1");
        ipmFrame = *ipmObj.getIPMFrame();

/*************      save output frames as pics         ***********/
//        if (counter < 10 )
//            frameName = "frame00" + to_string(counter) + ".png";
//        else if (counter < 100 )
//            frameName = "frame0" + to_string(counter) + ".png";
//        else
//            frameName = "frame" + to_string(counter) + ".png";
//        imwrite(frameName, ipmFrame);

        t2 = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(t2 - t1).count();
        allTime += duration;
        counter++;
    }
    cout << "Avg Exec Time : " << allTime/counter << "ms" << endl;
}







