#include "stream.h"

Stream::Stream() : width(0), height(0), fps(0)
{
    timer = new QTimer(this);
    stream_in = new StreamIn;
    stream_out = new StreamOut;
    connect(timer, SIGNAL(timeout()), this, SLOT(showFrames()));
    fsFrame = Q_NULLPTR;
    normal_default_screen = cv::Mat::zeros(480, 854, CV_8UC3);
    ipm_default_screen = cv::Mat::zeros(854, 480, CV_8UC3);
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
    cv::cvtColor(frames[MultiVideo::final_rgb], frames[MultiVideo::final_rgb], cv::COLOR_BGR2HSV);

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
    connect(stream_in, SIGNAL(endStream()), this, SLOT(initScreens()));
    initScreens();
}

void Stream::setIPMFrame(cv::Mat *f)
{
    frames[MultiVideo::ipm_rgb] = *f;
    frames[MultiVideo::ipm_bw] = *f;
}

void Stream::FullScreenFrame(int index)
{
    switch(index)
    {
    case 0:
    case 1:
        fsViewer->setGeometry(0, 0, 854, 480);
        fsViewer->setVideoSize(854, 480);
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

void Stream::setPointsToDraw(std::vector<cv::Vec2i> *pts)
{
    stream_out->setDrawingData(pts);
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
}

Stream::~Stream()
{

}
