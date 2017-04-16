#ifndef MULTIVIDEOVIEWER_H
#define MULTIVIDEOVIEWER_H

#include <QWidget>
#include "opencv_opengl_viewer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

#define VideosNum       5
#define LayoutsNum      4

namespace MultiVideoViewerWidget {
    enum videoType{normal, final_rgb, final_bw, ipm_rgb, ipm_bw};
}


class MultiVideoViewer : public QWidget
{
    Q_OBJECT

public:
    explicit MultiVideoViewer(QWidget *parent = 0);
    void setVideoSize(int w1, int h1, int w2, int h2);
    CVGLViewer *getVideoWidget(int index);


protected:
    CVGLViewer *videos[VideosNum];

private:
    QHBoxLayout *h_main_layout;
    QHBoxLayout *h_sec_layouts[LayoutsNum];
    QVBoxLayout *v_sec_layouts[LayoutsNum];

    void initContainer();
};

#endif // MULTIVIDEOVIEWER_H
