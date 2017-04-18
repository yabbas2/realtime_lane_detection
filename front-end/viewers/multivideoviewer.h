#ifndef MULTIVIDEOVIEWER_H
#define MULTIVIDEOVIEWER_H

#include <QWidget>
#include "opencv_opengl_viewer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

#define VideosNum       4
#define LayoutsNum      2

namespace MultiVideoViewerWidget {
    enum {normal, final_rgb, ipm_rgb, ipm_bw};
}


class MultiVideoViewer : public QWidget
{
    Q_OBJECT

public:
    explicit MultiVideoViewer(QWidget *parent = 0);
    void setVideoSize(int w1, int h1, int w2, int h2);
    void setVideoSize(const QSize &s1, const QSize &s2);
    CVGLViewer *getVideoWidget(int index);


protected:
    CVGLViewer *videos[VideosNum];

private:
    QVBoxLayout *v_main_layout;
    QHBoxLayout *h_sec_layouts[LayoutsNum];

};

#endif // MULTIVIDEOVIEWER_H
