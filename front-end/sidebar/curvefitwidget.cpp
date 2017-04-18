#include "curvefitwidget.h"

CurveFitWidget::CurveFitWidget(QWidget *parent) :
    QWidget(parent)
{
    curve_fit_gb = new QGroupBox(this);
    curve_fit_gb->setFixedSize(GroupBoxFixedWidth, GroupBoxFixedHeight);
    h_curve_fit_gb = new QHBoxLayout;
    curve_fit_gb->setTitle("Curve Fitting Parameters");
    curve_fit_gb->setLayout(h_curve_fit_gb);
    curve_fit_no_pts_slider = new QSlider(curve_fit_gb);
    curve_fit_no_pts_label = new QLabel(curve_fit_gb);
    v_curve_fit_labels = new QVBoxLayout;
    v_curve_fit_sliders = new QVBoxLayout;
    curve_fit_no_pts_label->setText("Points no.: 20");
    curve_fit_no_pts_slider->setOrientation(Qt::Horizontal);
    curve_fit_no_pts_slider->setTickPosition(QSlider::TicksAbove);
    curve_fit_no_pts_slider->setValue(20);
    curve_fit_no_pts_slider->setMaximum(100);
    curve_fit_no_pts_slider->setMinimum(10);
    v_curve_fit_labels->addWidget(curve_fit_no_pts_label);
    v_curve_fit_sliders->addWidget(curve_fit_no_pts_slider);
    h_curve_fit_gb->addLayout(v_curve_fit_labels);
    connect(curve_fit_no_pts_slider, SIGNAL(valueChanged(int)), this, SLOT(changeSliderValue()));
    curve_fit_y_label = new QLabel(curve_fit_gb);
    curve_fit_y_slider = new QSlider(curve_fit_gb);
    v_curve_fit_labels->addWidget(curve_fit_y_label);
    v_curve_fit_sliders->addWidget(curve_fit_y_slider);
    h_curve_fit_gb->addLayout(v_curve_fit_sliders);
    curve_fit_y_label->setText("Curve length: 100");
    curve_fit_y_slider->setOrientation(Qt::Horizontal);
    curve_fit_y_slider->setTickPosition(QSlider::TicksAbove);
    curve_fit_y_slider->setValue(100);
    curve_fit_y_slider->setMaximum(300);
    curve_fit_y_slider->setMinimum(10);
    connect(curve_fit_y_slider, SIGNAL(valueChanged(int)), this, SLOT(changeSliderValue()));
    curve_fit_no_pts_label->setFixedWidth((curve_fit_no_pts_label->width()));
    curve_fit_y_label->setFixedWidth((curve_fit_y_label->width()));
    this->setFixedSize(curve_fit_gb->size());
}

void CurveFitWidget::changeSliderValue()
{
    if (sender() == curve_fit_no_pts_slider)
        curve_fit_no_pts_label->setText("Points no.: " + QString::number(curve_fit_no_pts_slider->value()));
    else if (sender() == curve_fit_y_slider)
        curve_fit_y_label->setText("Curve length: " + QString::number(curve_fit_y_slider->value()));
}
