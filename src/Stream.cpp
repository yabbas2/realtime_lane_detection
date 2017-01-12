#include "Stream.hpp"

const char Stream::endStream = 'q';
Mat Stream::frame;
VideoCapture Stream::cap;
double Stream::fps = 0;
double Stream::framesNumber = 0;
vector<Vec4d>Stream::lanes = vector<Vec4d>(0, 0);

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
    while(true)
    {
        cap >> frame;
        if(frame.empty())
            break;
        if((waitKey((int)fps) & 0xFF) == (int)endStream)
            break;
        if(!lanes.empty())
            for(size_t i = 0; i < lanes.size(); i++)
                line(frame, Point((int)lanes[i][0], (int)lanes[i][1]),
                            Point((int)lanes[i][2], (int)lanes[i][3]),
                                                    Scalar(0,0,255), 3, CV_AA);
        imshow("Video", frame);  
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
void Stream::setLanesPositions(vector<Vec4d> l)
{
    lanes = l;
    return;
}