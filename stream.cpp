#include "stream.h"

Stream::Stream() : width(0), height(0), fps(0)
{
    timer = new QTimer(this);
    stream_in = new StreamIn;
    stream_out = new StreamOut;
    connect(timer, SIGNAL(timeout()), this, SLOT(showFrames()));
    connect(stream_in, SIGNAL(endStream()), this, SLOT(initScreens()));
    fsFrame = Q_NULLPTR;
    normal_default_screen = cv::Mat::zeros(480, 800, CV_8UC3);
    ipm_default_screen = cv::Mat::zeros(800, 480, CV_8UC3);
    updateDataLock = true;
}

void Stream::changeStreamInSource(QString source)
{
    if (timer->isActive())
        timer->stop();
    stream_in->stopStreamIn();
    streamInSource = source;
    stream_in->initStreamIn(streamInSource);
    stream_in->getVideoInfo(width, height, fps);
    qDebug() << "[STREAM] using framerate:" << fps;
    initScreens();
}

void Stream::reInitStream()
{
    if (timer->isActive())
        timer->stop();
    stream_in->stopStreamIn();
    initScreens();
}

void Stream::showFrames()
{
    frames[MultiVideo::normal_rgb] = stream_in->getFrame()->clone();
    frames[MultiVideo::final_rgb] = stream_in->getFrame()->clone();
//    if (! updateDataLock)
//    {
//        stream_out->drawFinalRGB(frames[MultiVideo::final_rgb]);
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

void Stream::pause_timers()
{
    stream_in->pauseStreamIn();
    timer->stop();
}

void Stream::start_timers()
{
    stream_in->startStreamIn();
    timer->start(static_cast<int> (1000/fps) + delayOffset);
}

void Stream::setViewers(MultiVideoViewer *m, fullScreenVideoViewer *f)
{
    multiViewer = m;
    fsViewer = f;
    connect(multiViewer->getVideoWidget(0), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(1), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(2), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(3), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    initScreens();
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

void Stream::setInfo(std::vector<cv::Vec2f> leftPoints, std::vector<cv::Vec2f> rightPoints, Vec2i ls, Vec2i rs)
{
    updateDataLock = true;
    if (leftPoints.size() > 0)
        this->leftPoints.clear();
    if (rightPoints.size() > 0)
        this->rightPoints.clear();
    for (unsigned int i = 0; i < leftPoints.size(); i++)
        this->leftPoints.push_back(Vec2i{(int)leftPoints[i][0], (int)leftPoints[i][1]});
    for (unsigned int i = 0; i < rightPoints.size(); i++)
        this->rightPoints.push_back(Vec2i{(int)rightPoints[i][0], (int)rightPoints[i][1]});
    Scalar l, r;
    if (ls[0] & ls[1])
        l = Scalar(0, 255, 0);
    else
        l = Scalar(0, 0, 255);
    if (rs[0] & rs[1])
        r = Scalar(0, 255, 0);
    else
        r = Scalar(0, 0, 255);
    if (this->leftPoints.size() > 0 && this->rightPoints.size() > 0)
    {
        stream_out->setDrawingData(&(this->leftPoints), &(this->rightPoints), l, r);
        updateDataLock = false;
    }
}

cv::Mat Stream::getFrame()
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
    connect(videoWidget, SIGNAL(pauseStreaming()), this, SLOT(pause_timers()));
    connect(videoWidget, SIGNAL(startStreaming()), this, SLOT(start_timers()));
    connect(sideBar->inputMethod, SIGNAL(pauseStreaming()), this, SLOT(pause_timers()));
    connect(sideBar->inputMethod, SIGNAL(startStreaming()), this, SLOT(start_timers()));
    connect(sideBar->inputMethod, SIGNAL(changeVideoSource(QString)), this, SLOT(changeStreamInSource(QString)));
    connect(sideBar->inputMethod, SIGNAL(reInit()), this, SLOT(initScreens()));
    setViewers(multiViewer, fsViewer);
}

Stream::~Stream()
{

}