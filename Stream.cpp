#include "Stream.hpp"

bool Stream::stopStream = false;
const char Stream::endStream = 'q';
Mat Stream::frame;
VideoCapture Stream::cap;
double Stream::fps = 0;
//mutex Stream::lock;

bool Stream::setVideoSource(string source)
{
    if(source.length() == 1)
        cap = VideoCapture(stoi(source));
    else
        cap = VideoCapture(source);
    if(!cap.isOpened())
        return false;
    fps = cap.get(CV_CAP_PROP_FPS);
    return true;
}
void Stream::videoInStream()
{
    cout << "[INFO] Start of streaming!" << endl;
    while(!stopStream)
    {
        //lock_guard<mutex> guard(lock);
        cap >> frame;
        if(frame.empty())
            stopStream = true;
        if((waitKey((int)fps) & 0xFF) == (int)endStream)
            stopStream = true;
    }
    cout << "[INFO] End of streaming!" << endl;
    cap.release();
    return;
}
Mat Stream::readFrame()
{
    return frame;
}
void Stream::videoOutStream()
{
    namedWindow("Video", 1);
    while(!stopStream)
    {
        //lock_guard<mutex> guard(lock);
        if(frame.empty())
            continue;
        imshow("Video", frame);
    }
    destroyWindow("Video");
    exit(0);
}

