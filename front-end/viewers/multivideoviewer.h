#ifndef MULTIVIDEOVIEWER_H
#define MULTIVIDEOVIEWER_H

#include <QWidget>
#include "opencvViewer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>

#define VideosNum       4
#define LayoutsNum      2
#define normalAspectRatio   0.5620608899
#define ipmAspectRatio      1.779166667

namespace MultiVideo {
    enum {normal_rgb, final_rgb, ipm_rgb, ipm_bw};
}


class MultiVideoViewer : public QWidget
{
    Q_OBJECT

public:
    explicit MultiVideoViewer(QWidget *parent = 0);
    void setVideoSize(int w1, int w2);
    CViewer *getVideoWidget(int index);

signals:
    void clickOnVideo(int index);

protected:
    CViewer *videos[VideosNum];

private:
    QVBoxLayout *v_main_layout;
    QHBoxLayout *h_sec_layouts[LayoutsNum];
    void paintEvent(QPaintEvent *);

};

#endif // MULTIVIDEOVIEWER_H
