#include "stream.h"

Stream::Stream() : width(0), height(0), fps(0)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(showFrames()));
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
    normalFrame = stream_in.getFrame();
    multiViewer->getVideoWidget(MultiVideoViewerWidget::normal)->showImage(*normalFrame);
    multiViewer->getVideoWidget(MultiVideoViewerWidget::final_rgb)->showImage(*normalFrame);
    multiViewer->getVideoWidget(MultiVideoViewerWidget::final_bw)->showImage(*normalFrame);
    multiViewer->getVideoWidget(MultiVideoViewerWidget::ipm_rgb)->showImage(*normalFrame);
    multiViewer->getVideoWidget(MultiVideoViewerWidget::ipm_bw)->showImage(*normalFrame);
}

void Stream::connectToFrontEnd(MainWindow *w)
{
    multiViewer = w->getMultiVideoViewerWidget();
}

void Stream::setPointsToDraw(std::vector<cv::Vec2i> *pts)
{
    stream_out.setDrawingData(pts);
}
