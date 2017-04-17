#include "multivideoviewer.h"

MultiVideoViewer::MultiVideoViewer(QWidget *parent) :
    QWidget(parent)
{
    videos[0] = new CVGLViewer(this);
    videos[1] = new CVGLViewer(this);
    videos[2] = new CVGLViewer(this);
    videos[3] = new CVGLViewer(this);

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

void MultiVideoViewer::setVideoSize(int w1, int h1, int w2, int h2)
{
    videos[0]->setFixedSize(w1, h1);
    videos[1]->setFixedSize(w1, h1);
    videos[2]->setFixedSize(w2, h2);
    videos[3]->setFixedSize(w2, h2);
}

CVGLViewer *MultiVideoViewer::getVideoWidget(int index)
{
    return videos[index];
}
