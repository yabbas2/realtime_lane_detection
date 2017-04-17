#include "side_bar.h"

Side_bar::Side_bar(QWidget *parent) :
    QWidget(parent)
{
    v_main = new QVBoxLayout;
    w = new QWidget(this);
    v_main_scroll_area = new QVBoxLayout;
    this->setLayout(v_main_scroll_area);
    QFont f( "Arial", 8, QFont::Normal);
    this->setFont(f);
    main_scroll_area = new QScrollArea(this);
    v_main_scroll_area->addWidget(main_scroll_area);
    main_scroll_area->setWidget(w);
    w->setLayout(v_main);
    main_scroll_area->setWidgetResizable(true);


    input_gb = new QGroupBox(w);
    browse_video = new QPushButton(input_gb);
    stop_video = new QPushButton(input_gb);
    start_camera = new QPushButton(input_gb);
    stop_camera = new QPushButton(input_gb);
    pause_camera = new QPushButton(input_gb);
    pause_video = new QPushButton(input_gb);
    start_video = new QPushButton(input_gb);
    input = new QComboBox(input_gb);
    input_list = new QStringList();
    h_input = new QHBoxLayout;
    v_input = new QVBoxLayout;
    buttons_spacer = new QSpacerItem(0, 50);
    h_input->addSpacerItem(buttons_spacer);
    input_gb->setFixedHeight(100);
    input_list->append("None");
    input_list->append("Camera");
    input_list->append("Video File") ;
    input->insertItems(0, *input_list);
    v_main->addWidget(input_gb);
    input_gb->setLayout(v_input);
    v_input->addWidget(input);
    v_input->addLayout(h_input);
    input_gb->setTitle("Input Method");
    browse_video->setVisible(false);
    stop_video->setVisible(false);
    start_camera->setVisible(false);
    stop_camera->setVisible(false);
    start_video->setVisible(false);
    pause_video->setVisible(false);
    pause_camera->setVisible(false);
    start_camera->setText("start");
    stop_camera->setText("stop");
    pause_camera->setText("pause");
    h_input->addWidget(start_camera);
    h_input->addWidget(pause_camera);
    h_input->addWidget(stop_camera);
    h_input->addWidget(browse_video);
    h_input->addWidget(start_video);
    h_input->addWidget(pause_video);
    h_input->addWidget(stop_video);
    browse_video->setText("browse");
    start_video->setText("start");
    stop_video->setText("stop");
    pause_video->setText("pause");
    connect(input, SIGNAL(activated(QString)), this, SLOT(comboBox(QString)));

    gaussian_mask_gb = new QGroupBox(w);
    v_gaussian_mask_gb = new QVBoxLayout;
    gaussian_mask_slider = new QSlider(gaussian_mask_gb);
    gaussian_mask_label = new QLabel(gaussian_mask_gb);
    h_layout_g = new QHBoxLayout;
    v_main->addWidget(gaussian_mask_gb);
    gaussian_mask_gb->setLayout(v_gaussian_mask_gb);
    v_gaussian_mask_gb->addLayout(h_layout_g);
    gaussian_mask_gb->setTitle("Gaussian Blur Mask");
    gaussian_mask_label->setText("Size: 1x1");
    gaussian_mask_slider->setOrientation(Qt::Horizontal);
    gaussian_mask_slider->setSingleStep(2);
    gaussian_mask_slider->setTickPosition(QSlider::TicksAbove);
    gaussian_mask_slider->setValue(1);
    gaussian_mask_slider->setMaximum(51);
    gaussian_mask_slider->setMinimum(1);
    h_layout_g->addWidget(gaussian_mask_label);
    h_layout_g->addWidget(gaussian_mask_slider);
    connect(gaussian_mask_slider, SIGNAL(valueChanged(int)), this, SLOT(changeSliderValue()));

    lsd_gb = new QGroupBox(w);
    h_lsd_gb = new QHBoxLayout;
    lsd_gb->setTitle("LSD Thresholds");
    v_main->addWidget(lsd_gb);
    lsd_thrs_len_slider = new QSlider(lsd_gb);
    lsd_thrs_len_label = new QLabel(lsd_gb);
    v_layout_lsd_sliders = new QVBoxLayout;
    v_layout_lsd_labels = new QVBoxLayout;
    h_lsd_two_dials = new QHBoxLayout;
    lsd_thrs_len_label->setText("Length: 30");
    lsd_thrs_len_slider->setOrientation(Qt::Horizontal);
    lsd_thrs_len_slider->setTickPosition(QSlider::TicksAbove);
    lsd_thrs_len_slider->setValue(30);
    lsd_thrs_len_slider->setMaximum(50);
    lsd_thrs_len_slider->setMinimum(5);
    v_layout_lsd_labels->addWidget(lsd_thrs_len_label);
    v_layout_lsd_sliders->addWidget(lsd_thrs_len_slider);
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

    reg_grow_gb = new QGroupBox(w);
    h_reg_grow_gb = new QHBoxLayout;
    v_layout_reg_grow_labels = new QVBoxLayout;
    v_layout_reg_grow_sliders = new QVBoxLayout;
    reg_grow_gb->setTitle("Region Growing Thresholds");
    v_main->addWidget(reg_grow_gb);
    reg_grow_gb->setLayout(h_reg_grow_gb);
    reg_grow_ang_dial = new QDial(reg_grow_gb);
    reg_grow_ang_label = new QLabel(reg_grow_gb);
    reg_grow_x_slider = new QSlider(reg_grow_gb);
    reg_grow_x_label = new QLabel(reg_grow_gb);
    reg_grow_x_label->setText("\nX difference: 40");
    reg_grow_x_slider->setOrientation(Qt::Horizontal);
    reg_grow_x_slider->setTickPosition(QSlider::TicksAbove);
    reg_grow_x_slider->setValue(40);
    reg_grow_x_slider->setMaximum(60);
    reg_grow_x_slider->setMinimum(1);
    v_layout_reg_grow_sliders->addWidget(reg_grow_x_slider);
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

    curve_fit_gb = new QGroupBox(w);
    h_curve_fit_gb = new QHBoxLayout;
    curve_fit_gb->setTitle("Curve Fitting Parameters");
    v_main->addWidget(curve_fit_gb);
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

    decision_make_gb = new QGroupBox(w);
    decision_make_y_label = new QLabel(decision_make_gb);
    decision_make_y_slider = new QSlider(decision_make_gb);
    h_decision_make_y = new QHBoxLayout;
    v_decision_make_gb = new QVBoxLayout;
    v_main->addWidget(decision_make_gb);
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

    input_gb->setFixedWidth(GroupBoxFixedWidth);
    gaussian_mask_gb->setFixedWidth(GroupBoxFixedWidth);
    decision_make_gb->setFixedWidth(GroupBoxFixedWidth);
    curve_fit_gb->setFixedWidth(GroupBoxFixedWidth);
    reg_grow_gb->setFixedWidth(GroupBoxFixedWidth);
    lsd_gb->setFixedWidth(GroupBoxFixedWidth);

    gaussian_mask_label->setFixedWidth((gaussian_mask_label->width()));
    lsd_thrs_ang_label->setFixedWidth((lsd_thrs_ang_label->width()));
    lsd_thrs_len_label->setFixedWidth((lsd_thrs_len_label->width()));
    reg_grow_ang_label->setFixedWidth((reg_grow_ang_label->width()));
    reg_grow_x_label->setFixedWidth((reg_grow_x_label->width()));
    curve_fit_no_pts_label->setFixedWidth((curve_fit_no_pts_label->width()));
    curve_fit_y_label->setFixedWidth((curve_fit_y_label->width()));
    decision_make_y_label->setFixedWidth((decision_make_y_label->width()));
}
void Side_bar::comboBox(QString item)
{
    if (item == "Camera"){
        browse_video->setVisible(false);
        stop_video->setVisible(false);
        pause_video->setVisible(false);
        start_video->setVisible(false);
        start_camera->setVisible(true);
        stop_camera->setVisible(true);
        pause_camera->setVisible(true);
    }
    else if (item == "Video File"){
        browse_video->setVisible(true);
        stop_video->setVisible(true);
        pause_video->setVisible(true);
        start_video->setVisible(true);
        start_camera->setVisible(false);
        stop_camera->setVisible(false);
        pause_camera->setVisible(false);

    }
    else if (item == "None"){
        browse_video->setVisible(false);
        stop_video->setVisible(false);
        pause_video->setVisible(false);
        start_video->setVisible(false);
        start_camera->setVisible(false);
        stop_camera->setVisible(false);
        pause_camera->setVisible(false);
    }


}

void Side_bar::changeSliderValue()
{
    if (sender() == gaussian_mask_slider)
        gaussian_mask_label->setText("Size: " + QString::number(gaussian_mask_slider->value()) + "x" + QString::number(gaussian_mask_slider->value()));
    else if (sender() == lsd_thrs_len_slider)
        lsd_thrs_len_label->setText("Length: " + QString::number(lsd_thrs_len_slider->value()));
    else if (sender() == lsd_thrs_ang_max_dial || sender()== lsd_thrs_ang_min_dial)
        lsd_thrs_ang_label->setText("Min. angle: " + QString::number(lsd_thrs_ang_min_dial->value())+ "\nMax. angle: " + QString::number(lsd_thrs_ang_max_dial->value()));
    else if (sender() == reg_grow_ang_dial)
        reg_grow_ang_label->setText("Angle difference: " + QString::number(reg_grow_ang_dial->value()));
    else if (sender() == reg_grow_x_slider)
        reg_grow_x_label->setText("\nX difference: " + QString::number(reg_grow_x_slider->value()));
    else if (sender() == curve_fit_no_pts_slider)
        curve_fit_no_pts_label->setText("Points no.: " + QString::number(curve_fit_no_pts_slider->value()));
    else if (sender() == curve_fit_y_slider)
        curve_fit_y_label->setText("Curve length: " + QString::number(curve_fit_y_slider->value()));
    else if (sender() == decision_make_y_slider)
        decision_make_y_label->setText("Y difference: " + QString::number(decision_make_y_slider->value()));

}

Side_bar::~Side_bar()
{

}
