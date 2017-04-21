#ifndef CURVEFITWIDGET_H
#define CURVEFITWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

#define CurveFitBoxFixedWidth  240
#define CurveFitBoxFixedHeight  90

class CurveFitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CurveFitWidget(QWidget *parent = 0);

private slots:
    void changeSliderValue();

private:
    QSlider *curve_fit_no_pts_slider;
    QSlider *curve_fit_y_slider;
    QGroupBox *curve_fit_gb;
    QHBoxLayout *h_curve_fit_gb;
    QVBoxLayout *v_curve_fit_labels;
    QVBoxLayout *v_curve_fit_sliders;
    QLabel *curve_fit_no_pts_label;
    QLabel *curve_fit_y_label;
};

#endif // CURVEFITWIDGET_H
