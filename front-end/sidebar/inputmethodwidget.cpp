#include "inputmethodwidget.h"

InputMethodWidget::InputMethodWidget(QWidget *parent) :
    QWidget(parent)
{
    input_gb = new QGroupBox(this);
    input_gb->setFixedSize(InputBoxFixedWidth, InputBoxFixedHeight);
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
    connect(browse_video, SIGNAL(clicked(bool)), this, SLOT(browseVideoClicked()));
    this->setFixedSize(input_gb->size());
}

void InputMethodWidget::comboBox(QString item)
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

void InputMethodWidget::browseVideoClicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Select Video"), DirectoryName, tr("Videos (*.mp4)"));
    qDebug() << "file:" + file_name;
}
