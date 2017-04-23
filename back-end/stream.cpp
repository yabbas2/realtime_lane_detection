#include "stream.h"

Stream::Stream() : width(0), height(0), fps(0)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(showFrames()));
    fsFrame =  NULL;
}

void Stream::changeStreamInSource(QString source)
{
    streamInSource = source;
    stream_in.initStreamIn(streamInSource);
    stream_in.getVideoInfo(width, height, fps);
    qDebug() << "using framerate:" << fps;
    timer.start(static_cast<int> (1000/fps) + 1);
}

void Stream::showFrames()
{
    frames[MultiVideoViewerWidget::normal] = stream_in.getFrame()->clone();
    frames[MultiVideoViewerWidget::final_rgb] = stream_in.getFrame()->clone();
    cv::cvtColor(frames[1], frames[1], cv::COLOR_BGR2GRAY);
    frames[MultiVideoViewerWidget::ipm_rgb] = stream_in.getFrame()->clone();
    cv::cvtColor(frames[2], frames[2], cv::COLOR_BGR2Lab);
    frames[MultiVideoViewerWidget::ipm_bw] = stream_in.getFrame()->clone();
    cv::cvtColor(frames[3], frames[3], cv::COLOR_BGR2HSV);
    multiViewer->getVideoWidget(MultiVideoViewerWidget::normal)->showImage(frames[0]);
    multiViewer->getVideoWidget(MultiVideoViewerWidget::final_rgb)->showImage(frames[1]);
    multiViewer->getVideoWidget(MultiVideoViewerWidget::ipm_rgb)->showImage(frames[2]);
    multiViewer->getVideoWidget(MultiVideoViewerWidget::ipm_bw)->showImage(frames[3]);
    if (fsFrame != NULL)
        fsViewer->getVideoWidget()->showImage(*fsFrame);
}

void Stream::connectFrontEndToStreamBackEnd(MainWindow *w)
{
    multiViewer = w->getMultiVideoViewerWidget();
    fsViewer = w->getFullScreenVideoViewerWidget();
    connect(multiViewer->getVideoWidget(0), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(1), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(2), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(3), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
}

void Stream::FullScreenFrame(int index)
{
    fsFrame = &(frames[index]);
}

void Stream::setPointsToDraw(std::vector<cv::Vec2i> *pts)
{
    stream_out.setDrawingData(pts);
}
