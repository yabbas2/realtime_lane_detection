#include "stream.h"

Stream::Stream() : width(0), height(0), fps(0)
{
    timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(showFrames()));
//    connect(stream_in, SIGNAL(endStream()), this, SLOT(initScreens()));
    fsFrame = Q_NULLPTR;
    normal_default_screen = cv::Mat::zeros(480, 800, CV_8UC3);
    ipm_default_screen = cv::Mat::zeros(800, 480, CV_8UC3);
    updateDataLock = true;
    updateFrame = false;
    colorRange = {0, 0, 255};
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
    updateFrame = false;
    initScreens();
}

void Stream::run()
{
    while (true)
    {
        if (!updateFrame)
            continue;
        cap >> inputFrame;
        if (!cap.grab())
        {
            reInitStream();
            continue;
        }
        frames[MultiVideo::normal_rgb] = inputFrame.clone();
        frames[MultiVideo::final_rgb] = inputFrame.clone();
//        if (!updateDataLock)
//        {
//            drawFinalRGB();
//        }
        cvtColor(frames[MultiVideo::final_rgb], frames[MultiVideo::final_rgb], COLOR_BGR2HSV);
        if (!frames[MultiVideo::normal_rgb].empty())
            multiViewer->getVideoWidget(MultiVideo::normal_rgb)->showImage(frames[0]);
        if (!frames[MultiVideo::final_rgb].empty())
            multiViewer->getVideoWidget(MultiVideo::final_rgb)->showImage(frames[1]);
        if (!frames[MultiVideo::ipm_rgb].empty())
            multiViewer->getVideoWidget(MultiVideo::ipm_rgb)->showImage(frames[2]);
        if (!frames[MultiVideo::ipm_bw].empty())
            multiViewer->getVideoWidget(MultiVideo::ipm_bw)->showImage(frames[3]);
        if (fsFrame != Q_NULLPTR)
            fsViewer->getVideoWidget()->showImage(*fsFrame);
        msleep(1000/fps);
    }
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
    frames[MultiVideo::normal_rgb] = inputFrame.clone();
    frames[MultiVideo::final_rgb] = inputFrame.clone();
//    if (! updateDataLock)
//    {
//        drawFinalRGB();
//    }
    cvtColor(frames[MultiVideo::final_rgb], frames[MultiVideo::final_rgb], COLOR_BGR2HSV);
    if (!frames[MultiVideo::normal_rgb].empty())
        multiViewer->getVideoWidget(MultiVideo::normal_rgb)->showImage(frames[0]);
    if (!frames[MultiVideo::final_rgb].empty())
        multiViewer->getVideoWidget(MultiVideo::final_rgb)->showImage(frames[1]);
    if (!frames[MultiVideo::ipm_rgb].empty())
        multiViewer->getVideoWidget(MultiVideo::ipm_rgb)->showImage(frames[2]);
    if (!frames[MultiVideo::ipm_bw].empty())
        multiViewer->getVideoWidget(MultiVideo::ipm_bw)->showImage(frames[3]);
    if (fsFrame != Q_NULLPTR)
        fsViewer->getVideoWidget()->showImage(*fsFrame);
}

void Stream::pauseStream()
{
    qDebug() << "[STREAM] stop streaming";
//    timer->stop();
    updateFrame = false;
}

void Stream::startStream()
{
    qDebug() << "[STREAM] pause streaming";
    updateFrame = true;
//    timer->start(static_cast<int> (1000/fps));
}

void Stream::drawFinalRGB()
{
    Mat processed = frames[MultiVideo::final_rgb].clone();
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
    polylines(frames[MultiVideo::final_rgb], leftPts, false, leftColor, 3, LINE_8);
    polylines(frames[MultiVideo::final_rgb], rightPts, false, rightColor, 3, LINE_8);
    inRange(processed, colorRange, colorRange, mask);
    findContours(mask, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    if (contours.empty())
        contours = prevContours;
    if (contours.size())
    {
        fillConvexPoly(processed, contours[0], Scalar(200, 0, 0), LINE_AA);
        addWeighted(processed, 0.5, frames[MultiVideo::final_rgb], 1 - 0.5, 0, frames[MultiVideo::final_rgb]);
        prevContours = contours;
    }
}

void Stream::setIPMFrame(cv::Mat *f)
{
    frames[MultiVideo::ipm_rgb] = f->clone();
//    frames[MultiVideo::ipm_bw] = f->clone();
}

void Stream::setIPMBW(cv::Mat *f)
{
//    frames[MultiVideo::ipm_rgb] = f->clone();
    frames[MultiVideo::ipm_bw] = f->clone();
}

void Stream::FullScreenFrame(int index)
{
    switch(index)
    {
    case 0:
    case 1:
        fsViewer->setGeometry(0, 0, 800, 480);
        fsViewer->setVideoSize(800, 480);
        break;
    case 2:
    case 3:
        fsViewer->setGeometry(292, 0, 270, 480);
        fsViewer->setVideoSize(270, 480);
        break;
    }
    fsFrame = &(frames[index]);
    fsViewer->getVideoWidget()->showImage(*fsFrame);
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
    return frames[MultiVideo::normal_rgb].clone();
}

void Stream::initScreens()
{
    frames[MultiVideo::normal_rgb] = normal_default_screen;
    frames[MultiVideo::final_rgb] = normal_default_screen;
    frames[MultiVideo::ipm_rgb] = ipm_default_screen;
    frames[MultiVideo::ipm_bw] = ipm_default_screen;
    multiViewer->getVideoWidget(MultiVideo::normal_rgb)->showImage(normal_default_screen);
    multiViewer->getVideoWidget(MultiVideo::final_rgb)->showImage(normal_default_screen);
    multiViewer->getVideoWidget(MultiVideo::ipm_rgb)->showImage(ipm_default_screen);
    multiViewer->getVideoWidget(MultiVideo::ipm_bw)->showImage(ipm_default_screen);
    timer->stop();
}

void Stream::connectToFrontEnd(MainWindow *w)
{
    multiViewer = w->getMultiVideoViewerWidget();
    fsViewer = w->getFullScreenVideoViewerWidget();
    sideBar = w->getSideBarWidget();
    videoWidget = w->getVideoWidget();
    connect(videoWidget, SIGNAL(pauseStreaming()), this, SLOT(pauseStream()));
    connect(videoWidget, SIGNAL(startStreaming()), this, SLOT(startStream()));
    connect(sideBar->inputMethod, SIGNAL(pauseStreaming()), this, SLOT(pauseStream()));
    connect(sideBar->inputMethod, SIGNAL(startStreaming()), this, SLOT(startStream()));
    connect(sideBar->inputMethod, SIGNAL(changeVideoSource(QString)), this, SLOT(changeStreamInSource(QString)));
    connect(sideBar->inputMethod, SIGNAL(reInit()), this, SLOT(initScreens()));
    connect(multiViewer->getVideoWidget(0), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(1), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(2), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(3), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    initScreens();
}

Stream::~Stream()
{

}
