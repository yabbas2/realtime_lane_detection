#ifndef DECISIONMAKEWIDGET_H
#define DECISIONMAKEWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#define GroupBoxFixedWidth  240
#define GroupBoxFixedHeight  65

class DecisionMakeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DecisionMakeWidget(QWidget *parent = 0);

private slots:
    void changeSliderValue();

private:
    QSlider *decision_make_y_slider;
    QGroupBox *decision_make_gb;
    QHBoxLayout *h_decision_make_y;
    QVBoxLayout *v_decision_make_gb;
    QLabel *decision_make_y_label;
};

#endif // DECISIONMAKEWIDGET_H
