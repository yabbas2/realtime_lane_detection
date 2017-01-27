#include "Processing.hpp"

const char Processing::endStream = 'q';
Mat Processing::normalFrame;
VideoCapture Processing::cap;
double Processing::fps = 0;
//double Processing::framesNumber = 0;
char Processing::videoSourceNum;

bool Processing::setVideoSource(string source)
{
    if(source.length() == 1)
        cap = VideoCapture(stoi(source));
    else
    {
        videoSourceNum = source[source.length()-5];
        cap = VideoCapture(source);
    }
    if(!cap.isOpened())
        return false;
    fps = cap.get(CV_CAP_PROP_FPS);
    //framesNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
    return true;
}

void Processing::videoIOStream()
{
    namedWindow("Video");
    cout << "[INFO] Start of streaming!" << endl;
    list<Vec4f>::iterator lane;
    Mat frameToShow;
    while(true)
    {
        cap >> normalFrame;
        frameToShow = normalFrame.clone();
        if(normalFrame.empty())
            break;
        if((waitKey((int)fps) & 0xFF) == (int)endStream)
            break;
        if(!detectedLanes.empty() && waitFlag)
        {
            for (lane = detectedLanes.begin(); lane != detectedLanes.end() && margin > 0; lane++)
            {
                line(frameToShow, Point((int)(*lane)[0], (int)(*lane)[1]),
                                     Point((int)(*lane)[2], (int)(*lane)[3]),
                                             Scalar(255, 0, 0), 3, CV_AA);
            }
            if(margin == 0)
                putText(frameToShow, "Searching for lanes..", Point(640/3, 20), FONT_HERSHEY_SIMPLEX, 0.7,
                        Scalar(0, 0, 255), 2, CV_AA);
            if(margin > 0)
                putText(frameToShow, lanesNumber+"-lane road" , Point(640/3, 20), FONT_HERSHEY_SIMPLEX, 0.7,
                        Scalar(0, 0, 255), 2, CV_AA);
            if(!arrow.empty())
                arrowedLine(frameToShow, Point(arrow[0], arrow[1]),
                                            Point(arrow[2], arrow[3]), Scalar(0, 255, 0), 5, CV_AA, 0, 0.3);
            waitFlag = false;
        }
        imshow("Video", frameToShow);
    }
    cout << "[INFO] End of streaming!" << endl;
    cap.release();
    destroyWindow("Video");
    exit(0);
}