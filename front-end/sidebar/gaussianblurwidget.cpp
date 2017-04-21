#include "gaussianblurwidget.h"

GaussianBlurWidget::GaussianBlurWidget(QWidget *parent) :
    QWidget(parent)
{
    gaussian_mask_gb = new QGroupBox(this);
    gaussian_mask_gb->setFixedSize(GaussianBoxFixedWidth, GaussianBoxFixedHeight);
    v_gaussian_mask_gb = new QVBoxLayout;
    gaussian_mask_slider = new QSlider(gaussian_mask_gb);
    gaussian_mask_label = new QLabel(gaussian_mask_gb);
    h_layout_g = new QHBoxLayout;
    gaussian_mask_gb->setLayout(v_gaussian_mask_gb);
    v_gaussian_mask_gb->addLayout(h_layout_g);
    gaussian_mask_gb->setTitle("Gaussian Blur Mask");
    gaussian_mask_label->setText("Size: 1x1");
    gaussian_mask_slider->setOrientation(Qt::Horizontal);
    gaussian_mask_slider->setSingleStep(2);
    gaussian_mask_slider->setTickPosition(QSlider::TicksAbove);
    gaussian_mask_slider->setValue(1);
    gaussian_mask_slider->setMaximum(51);
    gaussian_mask_slider->setMinimum(1);
    h_layout_g->addWidget(gaussian_mask_label);
    h_layout_g->addWidget(gaussian_mask_slider);
    connect(gaussian_mask_slider, SIGNAL(valueChanged(int)), this, SLOT(changeSliderValue()));
    gaussian_mask_label->setFixedWidth((gaussian_mask_label->width()));
    this->setFixedSize(gaussian_mask_gb->size());
}

void GaussianBlurWidget::changeSliderValue()
{
    if (sender() == gaussian_mask_slider)
        gaussian_mask_label->setText("Size: " + QString::number(gaussian_mask_slider->value()) + "x" +
                                     QString::number(gaussian_mask_slider->value()));
}
