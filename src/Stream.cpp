#include "Stream.hpp"

const char Stream::endStream = 'q';
Mat Stream::frame;
VideoCapture Stream::cap;
double Stream::fps = 0;
double Stream::framesNumber = 0;
list<Vec4d>Stream::lanes;

bool Stream::setVideoSource(string source)
{
    if(source.length() == 1)
        cap = VideoCapture(stoi(source));
    else
        cap = VideoCapture(source);
    if(!cap.isOpened())
        return false;
    fps = cap.get(CV_CAP_PROP_FPS);
    framesNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
    return true;
}
void Stream::videoIOStream()
{
    namedWindow("Video");
    cout << "[INFO] Start of streaming!" << endl;
    list<Vec4d>::iterator lane;
    while(true)
    {
        cap >> frame;
        if(frame.empty())
            break;
        if((waitKey((int)fps) & 0xFF) == (int)endStream)
            break;
        if(lanes.empty())
            goto init;
        for(lane = lanes.begin(); lane != lanes.end(); lane++)
            line(frame, Point((int)(*lane)[0], (int)(*lane)[1]),
                        Point((int)(*lane)[2], (int)(*lane)[3]),
                                                Scalar(0,255,0), 3, CV_AA);
init:   imshow("Video", frame);
    }
    cout << "[INFO] End of streaming!" << endl;
    cap.release();
    destroyWindow("Video");
    exit(0);
}
Mat Stream::readFrame()
{
    return frame.clone();
}
double Stream::getFrameCount()
{
    return framesNumber;
}
void Stream::setLanesPositions(list<Vec4d> * l)
{
    lanes = *l;
    return;
}