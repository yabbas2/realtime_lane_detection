#include "stream.h"

Stream::Stream(int argc, char *argv[]) :
    QApplication(argc, argv),
    width(0), height(0), fps(0)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showFrames()));
//    connect(stream_in, SIGNAL(endStream()), this, SLOT(initScreens()));
    fsFrame = Q_NULLPTR;
    normal_default_screen = cv::Mat::zeros(480, 800, CV_8UC3);
    ipm_default_screen = cv::Mat::zeros(800, 480, CV_8UC3);
    updateDataLock = true;
    colorRange = {0, 0, 255};
    ifGUI = new QDBusInterface("com.stage.gui", "/", "com.stage.gui", bus, this);
    connect(ifGUI, SIGNAL(setVideoSource(QString)), this, SLOT(changeStreamInSource(QString)));
    connect(ifGUI, SIGNAL(startStream()), this, SLOT(startStream()));
    connect(ifGUI, SIGNAL(pauseStream()), this, SLOT(pauseStream()));
}

void Stream::changeStreamInSource(QString source)
{
    reInitStream();
    streamInSource = source;
    if (!cap.open(source.toStdString()))
    {
        qDebug() << "[STREAM] Cannot open video streaming source";
        exit(1);
    }
    fps = static_cast<int> (cap.get(cv::CAP_PROP_FPS));
    qDebug() << "[STREAM] using framerate:" << fps;
}


void Stream::reInitStream()
{
    if (cap.isOpened())
        cap.release();
    initScreens();
}

void Stream::showFrames()
{
    cap >> inputFrame;
    if (!cap.grab())
    {
        reInitStream();
        timer->stop();
        return;
    }
    qDebug() << "wehooooooooooo";
    frames[stream::normal_rgb] = inputFrame.clone();
    frames[stream::final_rgb] = inputFrame.clone();
//    if (! updateDataLock)
//    {
//        drawFinalRGB();
//    }
    cvtColor(frames[stream::final_rgb], frames[stream::final_rgb], COLOR_BGR2HSV);
//    if (!frames[stream::normal_rgb].empty())
//        multiViewer->getVideoWidget(stream::normal_rgb)->showImage(frames[0]);
//    if (!frames[stream::final_rgb].empty())
//        multiViewer->getVideoWidget(stream::final_rgb)->showImage(frames[1]);
//    if (!frames[stream::ipm_rgb].empty())
//        multiViewer->getVideoWidget(stream::ipm_rgb)->showImage(frames[2]);
//    if (!frames[stream::ipm_bw].empty())
//        multiViewer->getVideoWidget(stream::ipm_bw)->showImage(frames[3]);
//    if (fsFrame != Q_NULLPTR)
//        fsViewer->getVideoWidget()->showImage(*fsFrame);
}

void Stream::pauseStream()
{
    qDebug() << "[STREAM] pause/stop streaming";
    timer->stop();
}

void Stream::startStream()
{
    qDebug() << "[STREAM] start/continue streaming";
    timer->start(static_cast<int> (1000/fps));
}

void Stream::drawFinalRGB()
{
    Mat processed = frames[stream::final_rgb].clone();
    Mat mask;
    vector<vector<Point>> contours;
    topPts.clear();
    bottomPts.clear();
    topPts.push_back(rightPts.at(0));
    bottomPts.push_back(rightPts.at(rightPts.size()-1));
    topPts.push_back(leftPts.at(0));
    bottomPts.push_back(leftPts.at(leftPts.size()-1));
    polylines(processed, topPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, bottomPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, leftPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, rightPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(frames[stream::final_rgb], leftPts, false, leftColor, 3, LINE_8);
    polylines(frames[stream::final_rgb], rightPts, false, rightColor, 3, LINE_8);
    inRange(processed, colorRange, colorRange, mask);
    findContours(mask, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    if (contours.empty())
        contours = prevContours;
    if (contours.size())
    {
        fillConvexPoly(processed, contours[0], Scalar(200, 0, 0), LINE_AA);
        addWeighted(processed, 0.5, frames[stream::final_rgb], 1 - 0.5, 0, frames[stream::final_rgb]);
        prevContours = contours;
    }
}

void Stream::setIPMFrame(cv::Mat *f)
{
    frames[stream::ipm_rgb] = f->clone();
//    frames[stream::ipm_bw] = f->clone();
}

void Stream::setIPMBW(cv::Mat *f)
{
//    frames[stream::ipm_rgb] = f->clone();
    frames[stream::ipm_bw] = f->clone();
}

void Stream::FullScreenFrame(int index)
{
//    fsFrame = &(frames[index]);
//    fsViewer->getVideoWidget()->showImage(*fsFrame);
}

void Stream::setInfo(vector<Vec2f> leftPoints, vector<Vec2f> rightPoints, Vec2i leftProp, Vec2i rightProp)
{
    updateDataLock = true;
    if (leftProp[0] & leftProp[1])
        leftColor = Scalar(0, 255, 0);
    else
        leftColor = Scalar(0, 0, 255);
    if (rightProp[0] & rightProp[1])
        rightColor = Scalar(0, 255, 0);
    else
        rightColor = Scalar(0, 0, 255);
    this->leftPts.clear();
    this->rightPts.clear();
    for (unsigned int i = 0; i < leftPoints.size(); i++)
        this->leftPts.push_back(Vec2i{(int)leftPoints[i][0], (int)leftPoints[i][1]});
    for (unsigned int i = 0; i < rightPoints.size(); i++)
        this->rightPts.push_back(Vec2i{(int)rightPoints[i][0], (int)rightPoints[i][1]});
    updateDataLock = false;
}


Mat Stream::getFrame()
{
    return frames[stream::normal_rgb].clone();
}

void Stream::initScreens()
{
    frames[stream::normal_rgb] = normal_default_screen;
    frames[stream::final_rgb] = normal_default_screen;
    frames[stream::ipm_rgb] = ipm_default_screen;
    frames[stream::ipm_bw] = ipm_default_screen;
    timer->stop();
}

Stream::~Stream()
{

}
