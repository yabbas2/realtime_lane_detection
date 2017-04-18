#include "lsdwidget.h"

LSDWidget::LSDWidget(QWidget *parent) :
    QWidget(parent)
{
    lsd_gb = new QGroupBox(this);
    lsd_gb->setFixedSize(GroupBoxFixedWidth, GroupBoxFixedHeight);
    h_lsd_gb = new QHBoxLayout;
    lsd_gb->setTitle("LSD Thresholds");
    lsd_thrs_len_slider = new QSlider(lsd_gb);
    lsd_thrs_len_label = new QLabel(lsd_gb);
    v_layout_lsd_sliders = new QVBoxLayout;
    v_layout_lsd_labels = new QVBoxLayout;
    h_lsd_two_dials = new QHBoxLayout;
    spacer1 = new QSpacerItem(0, 5);
    spacer2 = new QSpacerItem(0, 10);
    lsd_thrs_len_label->setText("Length: 30");
    lsd_thrs_len_slider->setOrientation(Qt::Horizontal);
    lsd_thrs_len_slider->setTickPosition(QSlider::TicksAbove);
    lsd_thrs_len_slider->setValue(30);
    lsd_thrs_len_slider->setMaximum(50);
    lsd_thrs_len_slider->setMinimum(5);
    v_layout_lsd_labels->addWidget(lsd_thrs_len_label);
    v_layout_lsd_sliders->addSpacerItem(spacer1);
    v_layout_lsd_sliders->addWidget(lsd_thrs_len_slider);
    v_layout_lsd_sliders->addSpacerItem(spacer2);
    connect(lsd_thrs_len_slider, SIGNAL(valueChanged(int)), this, SLOT(changeSliderValue()));
    lsd_thrs_ang_max_dial = new QDial(lsd_gb);
    lsd_thrs_ang_min_dial = new QDial(lsd_gb);
    lsd_thrs_ang_label = new QLabel(lsd_gb);
    lsd_thrs_ang_label->setText("Min. angle: 70\nMax. angle: 110");
    h_lsd_two_dials->addWidget(lsd_thrs_ang_min_dial);
    h_lsd_two_dials->addWidget(lsd_thrs_ang_max_dial);
    v_layout_lsd_labels->addWidget(lsd_thrs_ang_label);
    v_layout_lsd_sliders->addLayout(h_lsd_two_dials);
    h_lsd_gb->addLayout(v_layout_lsd_labels);
    h_lsd_gb->addLayout(v_layout_lsd_sliders);
    lsd_gb->setLayout(h_lsd_gb);
    lsd_thrs_ang_max_dial->setNotchesVisible(true);
    lsd_thrs_ang_min_dial->setNotchesVisible(true);
    lsd_thrs_ang_max_dial->setValue(110);
    lsd_thrs_ang_min_dial->setValue(70);
    lsd_thrs_ang_max_dial->setMaximum(150);
    lsd_thrs_ang_max_dial->setMinimum(90);
    lsd_thrs_ang_min_dial->setMaximum(100);
    lsd_thrs_ang_min_dial->setMinimum(50);
    connect(lsd_thrs_ang_max_dial, SIGNAL(valueChanged(int)), this, SLOT(changeSliderValue()));
    connect(lsd_thrs_ang_min_dial, SIGNAL(valueChanged(int)), this, SLOT(changeSliderValue()));
    lsd_thrs_ang_label->setFixedWidth((lsd_thrs_ang_label->width()));
    lsd_thrs_len_label->setFixedWidth((lsd_thrs_len_label->width()));
    this->setFixedSize(lsd_gb->size());
}

void LSDWidget::changeSliderValue()
{
    if (sender() == lsd_thrs_len_slider)
        lsd_thrs_len_label->setText("Length: " + QString::number(lsd_thrs_len_slider->value()));
    else if (sender() == lsd_thrs_ang_max_dial || sender()== lsd_thrs_ang_min_dial)
        lsd_thrs_ang_label->setText("Min. angle: " + QString::number(lsd_thrs_ang_min_dial->value())+ "\nMax. angle: " + QString::number(lsd_thrs_ang_max_dial->value()));
}
