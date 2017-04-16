#include "multivideoviewer.h"

MultiVideoViewer::MultiVideoViewer(QWidget *parent) :
    QWidget(parent)
{
    videos[0] = new CVGLViewer(this);
    videos[1] = new CVGLViewer(this);
    videos[2] = new CVGLViewer(this);
    videos[3] = new CVGLViewer(this);
    videos[4] = new CVGLViewer(this);

    h_main_layout = new QHBoxLayout;
    h_sec_layouts[0] = new QHBoxLayout;
    h_sec_layouts[1] = new QHBoxLayout;
    h_sec_layouts[2] = new QHBoxLayout;
    h_sec_layouts[3] = new QHBoxLayout;
    v_sec_layouts[0] = new QVBoxLayout;
    v_sec_layouts[1] = new QVBoxLayout;
    v_sec_layouts[2] = new QVBoxLayout;
    v_sec_layouts[3] = new QVBoxLayout;

    this->setLayout(h_main_layout);

    initContainer();
}

void MultiVideoViewer::initContainer()
{
    for (int i = 0, j = 0; i < LayoutsNum && j < VideosNum; ++i, ++j)
    {
        h_main_layout->addLayout(h_sec_layouts[i]);
        h_sec_layouts[i]->addLayout(v_sec_layouts[i]);
        v_sec_layouts[i]->addWidget(videos[j]);
        if (i == 1 && j == 1)  // case of showing two videos in the same vertical layout
            v_sec_layouts[i]->addWidget(videos[++j]);
    }
}

void MultiVideoViewer::setVideoSize(int w1, int h1, int w2, int h2)
{
    videos[0]->setFixedSize(w1, h1);
    videos[1]->setFixedSize(w1, h1);
    videos[2]->setFixedSize(w1, h1);
    videos[3]->setFixedSize(w2, h2);
    videos[4]->setFixedSize(w2, h2);
}

CVGLViewer *MultiVideoViewer::getVideoWidget(int index)
{
    return videos[index];
}
