#ifndef REGIONGROWWIDGET_H
#define REGIONGROWWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDial>
#include <QSpacerItem>

#define GroupBoxFixedWidth  240
#define GroupBoxFixedHeight  125

class RegionGrowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegionGrowWidget(QWidget *parent = 0);

private slots:
    void changeSliderValue();

private:
    QSlider *reg_grow_x_slider;
    QDial *reg_grow_ang_dial;
    QGroupBox *reg_grow_gb;
    QHBoxLayout *h_reg_grow_gb;
    QVBoxLayout *v_layout_reg_grow_labels;
    QVBoxLayout *v_layout_reg_grow_sliders;
    QLabel *reg_grow_ang_label;
    QLabel *reg_grow_x_label;
    QSpacerItem *spacer1;
    QSpacerItem *spacer2;
};

#endif // REGIONGROWWIDGET_H
