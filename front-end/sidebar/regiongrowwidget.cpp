#include "regiongrowwidget.h"

RegionGrowWidget::RegionGrowWidget(QWidget *parent) :
    QWidget(parent)
{
    reg_grow_gb = new QGroupBox(this);
    reg_grow_gb->setFixedSize(RegGrowBoxFixedWidth, RegGrowBoxFixedHeight);
    h_reg_grow_gb = new QHBoxLayout;
    v_layout_reg_grow_labels = new QVBoxLayout;
    v_layout_reg_grow_sliders = new QVBoxLayout;
    spacer1 = new QSpacerItem(0, 5);
    spacer2 = new QSpacerItem(0, 10);
    reg_grow_gb->setTitle("Region Growing Thresholds");
    reg_grow_gb->setLayout(h_reg_grow_gb);
    reg_grow_ang_dial = new QDial(reg_grow_gb);
    reg_grow_ang_label = new QLabel(reg_grow_gb);
    reg_grow_x_slider = new QSlider(reg_grow_gb);
    reg_grow_x_label = new QLabel(reg_grow_gb);
    reg_grow_x_label->setText("X difference: 40");
    reg_grow_x_slider->setOrientation(Qt::Horizontal);
    reg_grow_x_slider->setTickPosition(QSlider::TicksAbove);
    reg_grow_x_slider->setValue(40);
    reg_grow_x_slider->setMaximum(60);
    reg_grow_x_slider->setMinimum(1);
    v_layout_reg_grow_sliders->addSpacerItem(spacer1);
    v_layout_reg_grow_sliders->addWidget(reg_grow_x_slider);
    v_layout_reg_grow_sliders->addSpacerItem(spacer2);
    v_layout_reg_grow_labels->addWidget(reg_grow_x_label);
    connect(reg_grow_x_slider, SIGNAL(valueChanged(int)), this, SLOT(changeSliderValue()));
    reg_grow_ang_dial->setNotchesVisible(true);
    reg_grow_ang_dial->setValue(10);
    reg_grow_ang_dial->setMaximum(20);
    reg_grow_ang_dial->setMinimum(1);
    reg_grow_ang_label->setText("Angle difference: 10");
    h_reg_grow_gb->addLayout(v_layout_reg_grow_labels);
    h_reg_grow_gb->addLayout(v_layout_reg_grow_sliders);
    v_layout_reg_grow_labels->addWidget(reg_grow_ang_label);
    v_layout_reg_grow_sliders->addWidget(reg_grow_ang_dial);
    connect(reg_grow_ang_dial, SIGNAL(valueChanged(int)), this, SLOT(changeSliderValue()));
    reg_grow_ang_label->setFixedWidth((reg_grow_ang_label->width()));
    reg_grow_x_label->setFixedWidth((reg_grow_x_label->width()));
    this->setFixedSize(reg_grow_gb->size());
}

void RegionGrowWidget::changeSliderValue()
{
    if (sender() == reg_grow_ang_dial)
        reg_grow_ang_label->setText("Angle difference: " + QString::number(reg_grow_ang_dial->value()));
    else if (sender() == reg_grow_x_slider)
        reg_grow_x_label->setText("X difference: " + QString::number(reg_grow_x_slider->value()));
}
