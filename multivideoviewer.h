#ifndef MULTIVIDEOVIEWER_H
#define MULTIVIDEOVIEWER_H

#include <QWidget>
#include "opencv_opengl_viewer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

#define normal_video_rgb        videos[0]
#define final_video_rgb         videos[1]
#define final_video_bw          videos[2]
#define ipm_video_rgb           videos[3]
#define ipm_video_bw            videos[4]

class MultiVideoViewer : public QWidget
{
    Q_OBJECT

public:
    explicit MultiVideoViewer(QWidget *parent = 0);

protected:
    CVGLViewer *videos[5];

private:
    QHBoxLayout *h_main_layout;
    QHBoxLayout *h_sec_layouts[4];
    QVBoxLayout *v_sec_layouts[4];

    void init_container();
};

#endif // MULTIVIDEOVIEWER_H
