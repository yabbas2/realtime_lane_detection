#include "fullscreenvideoviewer.h"

fullScreenVideoViewer::fullScreenVideoViewer(QWidget *parent) :
    QWidget(parent)
{
    fsVideo = new CViewer(0, this);
}

CViewer *fullScreenVideoViewer::getVideoWidget()
{
    return fsVideo;
}

void fullScreenVideoViewer::setVideoSize(int w, int h)
{
    fsVideo->setSize(w, h);
}
