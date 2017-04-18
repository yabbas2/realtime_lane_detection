#ifndef GAUSSIANBLURWIDGET_H
#define GAUSSIANBLURWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#define GroupBoxFixedWidth  240
#define GroupBoxFixedHeight  65

class GaussianBlurWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GaussianBlurWidget(QWidget *parent = 0);

private slots:
    void changeSliderValue();

private:
    QSlider *gaussian_mask_slider;
    QGroupBox *gaussian_mask_gb;
    QHBoxLayout *h_layout_g;
    QVBoxLayout *v_gaussian_mask_gb;
    QLabel *gaussian_mask_label;
};

#endif // GAUSSIANBLURWIDGET_H
