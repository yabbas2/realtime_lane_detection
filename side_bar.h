#ifndef SIDE_BAR_H
#define SIDE_BAR_H

#include <QtWidgets/QWidget>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <QLabel>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDial>
#include <QGroupBox>
#include <QStyleOptionGroupBox>
#include <QComboBox>
#include <QStringList>
#include <QPushButton>
#include <QFont>
#include <QScrollArea>
#include <QScrollBar>
#include <QSpacerItem>

#define GroupBoxFixedWidth  240

class Side_bar : public QWidget
{
    Q_OBJECT

public:
    explicit Side_bar(QWidget *parent = 0);
    ~Side_bar();

public slots:
    void changeSliderValue();
    void comboBox(QString item);


private:
    QWidget * w;
    QScrollArea *main_scroll_area;
    QSlider *gaussian_mask_slider;
    QSlider *lsd_thrs_len_slider;
    QSlider *reg_grow_x_slider;
    QSlider *curve_fit_no_pts_slider;
    QSlider *curve_fit_y_slider;
    QSlider *decision_make_y_slider;
    QDial *lsd_thrs_ang_min_dial;
    QDial *lsd_thrs_ang_max_dial;
    QDial *reg_grow_ang_dial;
    QGroupBox *gaussian_mask_gb;
    QGroupBox *lsd_gb;
    QGroupBox *reg_grow_gb;
    QGroupBox *curve_fit_gb;
    QGroupBox *decision_make_gb;
    QGroupBox *input_gb;
    QComboBox *input;
    QStringList *input_list;
    QHBoxLayout *h_lsd_gb;
    QHBoxLayout *h_layout_g;
    QHBoxLayout *h_layout_reg_grow_ang;
    QHBoxLayout *h_reg_grow_x;
    QHBoxLayout *h_input;
    QHBoxLayout *h_input_extra_buttons;
    QHBoxLayout *h_curve_fit_gb;
    QHBoxLayout *h_decision_make_y;
    QHBoxLayout *h_lsd_two_dials;
    QVBoxLayout *v_main;
    QVBoxLayout *v_main_scroll_area;
    QVBoxLayout *v_input;
    QVBoxLayout *v_gaussian_mask_gb;
    QVBoxLayout *v_layout_lsd_labels;
    QVBoxLayout *v_layout_lsd_sliders;
    QVBoxLayout *v_reg_grow_gb;
    QVBoxLayout *v_curve_fit_labels;
    QVBoxLayout *v_curve_fit_sliders;
    QVBoxLayout *v_decision_make_gb;
    QLabel *gaussian_mask_label;
    QLabel *lsd_thrs_len_label;
    QLabel *lsd_thrs_ang_label;
    QLabel *reg_grow_ang_label;
    QLabel *reg_grow_x_label;
    QLabel *curve_fit_no_pts_label;
    QLabel *curve_fit_y_label;
    QLabel *decision_make_y_label;
    QPushButton *start_camera;
    QPushButton *stop_camera;
    QPushButton *pause_camera;
    QPushButton *browse_video;
    QPushButton *stop_video;
    QPushButton *start_video;
    QPushButton *pause_video;
    QSpacerItem *buttons_spacer;
};

#endif // SIDE_BAR_H
