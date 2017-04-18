#include "decisionmakewidget.h"

DecisionMakeWidget::DecisionMakeWidget(QWidget *parent) :
    QWidget(parent)
{
    decision_make_gb = new QGroupBox(this);
    decision_make_gb->setFixedSize(GroupBoxFixedWidth, GroupBoxFixedHeight);
    decision_make_y_label = new QLabel(decision_make_gb);
    decision_make_y_slider = new QSlider(decision_make_gb);
    h_decision_make_y = new QHBoxLayout;
    v_decision_make_gb = new QVBoxLayout;
    v_decision_make_gb->addLayout(h_decision_make_y);
    decision_make_gb->setLayout(v_decision_make_gb);
    decision_make_gb->setTitle("Decision Making Parameters");
    decision_make_y_label->setText("Y difference: 150");
    decision_make_y_slider->setOrientation(Qt::Horizontal);
    decision_make_y_slider->setTickPosition(QSlider::TicksAbove);
    decision_make_y_slider->setValue(150);
    decision_make_y_slider->setMaximum(250);
    decision_make_y_slider->setMinimum(50);
    h_decision_make_y->addWidget(decision_make_y_label);
    h_decision_make_y->addWidget(decision_make_y_slider);
    connect(decision_make_y_slider, SIGNAL(valueChanged(int)), this, SLOT(changeSliderValue()));
    decision_make_y_label->setFixedWidth((decision_make_y_label->width()));
    this->setFixedSize(decision_make_gb->size());
}

void DecisionMakeWidget::changeSliderValue()
{
    if (sender() == decision_make_y_slider)
        decision_make_y_label->setText("Y difference: " + QString::number(decision_make_y_slider->value()));
}
