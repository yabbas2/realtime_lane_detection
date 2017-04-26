#include "pipeline.h"

Pipeline::Pipeline()
{
    streamObj = new Stream;
    ipmObj = new IPM;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(exec()));
}

void Pipeline::exec()
{
    normalFrame = streamObj->getFrame();
    if (normalFrame.empty())
        return;
    ipmObj->transform(normalFrame, "sample1");
    ipmFrame = ipmObj->getIPMFrame();
    streamObj->setIPMFrame(ipmFrame);
}

void Pipeline::connectFrontEndToBackEnd(MainWindow *w)
{
    multiViewer = w->getMultiVideoViewerWidget();
    fsViewer = w->getFullScreenVideoViewerWidget();
    videoWidget = w->getVideoWidget();
    sideBar = w->getSideBarWidget();
    connect(videoWidget, SIGNAL(pauseStreaming()), this, SLOT(pause_timers()));
    connect(videoWidget, SIGNAL(startStreaming()), this, SLOT(start_timers()));
    connect(sideBar->inputMethod, SIGNAL(pauseStreaming()), this, SLOT(pause_timers()));
    connect(sideBar->inputMethod, SIGNAL(startStreaming()), this, SLOT(start_timers()));
    connect(sideBar->inputMethod, SIGNAL(changeVideoSource(QString)), streamObj, SLOT(changeStreamInSource(QString)));
    streamObj->setViewers(multiViewer, fsViewer);
}

void Pipeline::pause_timers()
{
    timer->stop();
    streamObj->pause_timers();
}

void Pipeline::start_timers()
{
    streamObj->start_timers();
    timer->start(static_cast<int> (1000/streamObj->fps) + delayOffset);
}

Pipeline::~Pipeline()
{

}
