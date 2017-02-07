#include "Processing.hpp"

const char Processing::endStream = 'q';
Mat Processing::normalFrame;
Mat Processing::frameToShow;
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
    cout << "[INFO] Start of streaming!" << endl;
#if writeVideo
    VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),fps, Size(640,360),true);
#endif
    //Mat finalFrame;
    while(true)
    {
        cap >> normalFrame;
        frameToShow = normalFrame.clone();
        if(normalFrame.empty())
            break;
        if((waitKey((int)fps) & 0xFF) == (int)endStream)
            break;
        draw();
#if show_final
        //hconcat(normalFrame, frameToShow, finalFrame);
#if writeVideo
        video.write(frameToShow);
#endif
        imshow("Real-time Lane Detection", frameToShow);
#endif
#if show_ipm
        if(!ipmFrame.empty()) imshow("IPM", ipmFrame);
#endif
    }
    cout << "[INFO] End of streaming!" << endl;
    cap.release();
    destroyWindow("Video");
    exit(0);
}

void Processing::draw()
{
    list<Vec4f>::iterator lane;
    if(!detectedLanes.empty() && waitFlag)
    {
        for (lane = detectedLanes.begin(); lane != detectedLanes.end() && lineMargin > 0; lane++)
        {
            line(frameToShow, Point((int)(*lane)[0], (int)(*lane)[1]),
                 Point((int)(*lane)[2], (int)(*lane)[3]),
                 Scalar(255, 0, 0), 3, CV_AA);
        }
        rectangle(frameToShow, Point(0, 0), Point(200, 50), Scalar(0, 0, 0), 1, CV_AA);
        putText(frameToShow, "status: ", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.4,
                Scalar(255, 0, 0), 1, CV_AA);
        putText(frameToShow, "AFPS: ", Point(10, 40), FONT_HERSHEY_SIMPLEX, 0.4,
                Scalar(255, 0, 0), 1, CV_AA);
        if(lineMargin == 0)
            putText(frameToShow, "searching for lanes..", Point(60, 20), FONT_HERSHEY_SIMPLEX, 0.4,
                    Scalar(0, 0, 255), 1, CV_AA);
        if(lineMargin > 0)
            putText(frameToShow, lanesNumber+"-lane road" , Point(60, 20), FONT_HERSHEY_SIMPLEX, 0.4,
                    Scalar(0, 255, 0), 1, CV_AA);
        if(afps >= fps)
            putText(frameToShow, to_string(afps), Point(60, 40), FONT_HERSHEY_SIMPLEX, 0.4,
                    Scalar(0, 255, 0), 1, CV_AA);
        else
            putText(frameToShow, to_string(afps), Point(60, 40), FONT_HERSHEY_SIMPLEX, 0.4,
                    Scalar(0, 0, 255), 1, CV_AA);
        if(!arrow.empty() && lineMargin > 0)
            arrowedLine(frameToShow, Point(arrow[0], arrow[1]),
                        Point(arrow[2], arrow[3]), Scalar(0, 255, 0), 5, CV_AA, 0, 0.3);
        waitFlag = false;
    }
}