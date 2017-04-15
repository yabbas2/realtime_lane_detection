#ifndef MULTIVIDEOVIEWER_H
#define MULTIVIDEOVIEWER_H

#include <QWidget>
#include "opencv_opengl_viewer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

class MultiVideoViewer : public QWidget
{
    Q_OBJECT

public:
    explicit MultiVideoViewer(QWidget *parent = 0);
    void setVideoSize(int w1, int h1, int w2, int h2);

protected:
    CVGLViewer *videos[5];

private:
    QHBoxLayout *h_main_layout;
    QHBoxLayout *h_sec_layouts[4];
    QVBoxLayout *v_sec_layouts[4];

    void initContainer();
};

#endif // MULTIVIDEOVIEWER_H
