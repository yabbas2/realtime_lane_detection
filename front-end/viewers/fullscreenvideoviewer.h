#ifndef FULLSCREENVIDEOVIEWER_H
#define FULLSCREENVIDEOVIEWER_H

#include <QWidget>
#include <QDebug>
#include "opencv_opengl_viewer.h"

class fullScreenVideoViewer : public QWidget
{
    Q_OBJECT

public:
    explicit fullScreenVideoViewer(QWidget *parent = 0);
    CVGLViewer *getVideoWidget();
    void setVideoSize(int w, int h);
    void setVideoSize(const QSize &s);

protected:
    CVGLViewer *fsVideo;

};

#endif // FULLSCREENVIDEOVIEWER_H
