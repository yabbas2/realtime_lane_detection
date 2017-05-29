#include "fullscreenvideoviewer.h"

fullScreenVideoViewer::fullScreenVideoViewer(QWidget *parent) :
    QWidget(parent)
{
    fsVideo = new CVGLViewer(0, this);
}

CVGLViewer *fullScreenVideoViewer::getVideoWidget()
{
    return fsVideo;
}

void fullScreenVideoViewer::setVideoSize(int w, int h)
{
    fsVideo->setFixedSize(w, h);
}

void fullScreenVideoViewer::setVideoSize(const QSize &s)
{
    fsVideo->setFixedSize(s);
}
