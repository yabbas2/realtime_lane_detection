#ifndef LSDWIDGET_H
#define LSDWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDial>
#include <QSpacerItem>

#define LSDBoxFixedWidth  240
#define LSDBoxFixedHeight  125

class LSDWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LSDWidget(QWidget *parent = 0);

private slots:
    void changeSliderValue();

private:
    QSlider *lsd_thrs_len_slider;
    QDial *lsd_thrs_ang_min_dial;
    QDial *lsd_thrs_ang_max_dial;
    QGroupBox *lsd_gb;
    QHBoxLayout *h_lsd_gb;
    QHBoxLayout *h_lsd_two_dials;
    QVBoxLayout *v_layout_lsd_labels;
    QVBoxLayout *v_layout_lsd_sliders;
    QLabel *lsd_thrs_len_label;
    QLabel *lsd_thrs_ang_label;
    QSpacerItem *spacer1;
    QSpacerItem *spacer2;
};

#endif // LSDWIDGET_H
