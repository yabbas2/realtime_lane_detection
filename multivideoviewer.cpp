#include "multivideoviewer.h"

MultiVideoViewer::MultiVideoViewer(QWidget *parent) :
    QWidget(parent)
{
    videos[0] = new CVGLViewer(this);
    videos[1] = new CVGLViewer(this);
    videos[2] = new CVGLViewer(this);
    videos[3] = new CVGLViewer(this);
    videos[4] = new CVGLViewer(this);

    videos[0]->setFixedSize(200, 200);
    videos[1]->setFixedSize(200, 200);
    videos[2]->setFixedSize(200, 200);
    videos[3]->setFixedSize(200, 200);
    videos[4]->setFixedSize(200, 200);

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

    init_container();
}


void MultiVideoViewer::init_container()
{
    for (int i = 0, j = 0; i < 4 && j < 5; ++i, ++j)
    {
        h_main_layout->addLayout(h_sec_layouts[i]);
        h_sec_layouts[i]->addLayout(v_sec_layouts[i]);
        v_sec_layouts[i]->addWidget(videos[j]);
        if (i == 1 && j == 1)
            v_sec_layouts[i]->addWidget(videos[++j]);
    }
}
