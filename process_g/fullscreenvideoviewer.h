#ifndef FULLSCREENVIDEOVIEWER_H
#define FULLSCREENVIDEOVIEWER_H

#include <QWidget>
#include "opencvViewer.h"

class fullScreenVideoViewer : public QWidget
{
    Q_OBJECT

public:
    explicit fullScreenVideoViewer(QWidget *parent = 0);
    CViewer *getVideoWidget();
    void setVideoSize(int w, int h);

protected:
    CViewer *fsVideo;
};

#endif // FULLSCREENVIDEOVIEWER_H
