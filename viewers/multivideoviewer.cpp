#include "multivideoviewer.h"

MultiVideoViewer::MultiVideoViewer(QWidget *parent) :
    QWidget(parent)
{
    videos[0] = new CViewer(0, this);
    videos[1] = new CViewer(1, this);
    videos[2] = new CViewer(2, this);
    videos[3] = new CViewer(3, this);

    v_main_layout = new QVBoxLayout;
    h_sec_layouts[0] = new QHBoxLayout;
    h_sec_layouts[1] = new QHBoxLayout;

    this->setLayout(v_main_layout);

    h_sec_layouts[0]->addWidget(videos[0]);
    h_sec_layouts[0]->addWidget(videos[1]);
    h_sec_layouts[1]->addWidget(videos[2]);
    h_sec_layouts[1]->addWidget(videos[3]);
    v_main_layout->addLayout(h_sec_layouts[0]);
    v_main_layout->addLayout(h_sec_layouts[1]);
}

void MultiVideoViewer::setVideoSize(int w1, int w2)
{
    videos[0]->setSize(w1, static_cast<int> (w1*normalAspectRatio));
    videos[1]->setSize(w1, static_cast<int> (w1*normalAspectRatio));
    videos[2]->setSize(w2, static_cast<int> (w2*ipmAspectRatio));
    videos[3]->setSize(w2, static_cast<int> (w2*ipmAspectRatio));
}

CViewer *MultiVideoViewer::getVideoWidget(int index)
{
    return videos[index];
}

void MultiVideoViewer::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
