#include "inputmethodwidget.h"

InputMethodWidget::InputMethodWidget(QWidget *parent) :
    QWidget(parent)
{
    input_gb = new QGroupBox(this);
    input_gb->setFixedSize(InputBoxFixedWidth, InputBoxFixedHeight);
    browse_video = new QPushButton(input_gb);
    pause_video = new QPushButton(input_gb);
    start_video = new QPushButton(input_gb);
    input = new QComboBox(input_gb);
    input_list = new QStringList();
    h_input = new QHBoxLayout;
    v_input = new QVBoxLayout;
    buttons_spacer = new QSpacerItem(0, 50);
    chosen_video_re.setPattern("[youtubekittiudacity]+_video[0-9]+");
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
    start_video->setVisible(false);
    pause_video->setVisible(false);
    h_input->addWidget(browse_video);
    h_input->addWidget(start_video);
    h_input->addWidget(pause_video);
    browse_video->setIcon(QIcon(":/icons/resources/buttons/browse.png"));
    start_video->setIcon(QIcon(":/icons/resources/buttons/play.png"));
    pause_video->setIcon(QIcon(":/icons/resources/buttons/pause.png"));
    connect(input, SIGNAL(activated(QString)), this, SLOT(comboBox(QString)));
    connect(pause_video, SIGNAL(clicked(bool)), this, SLOT(pause()));
    connect(start_video, SIGNAL(clicked(bool)), this, SLOT(play()));
    connect(browse_video, SIGNAL(clicked(bool)), this, SLOT(browseVideo()));
    this->setFixedSize(input_gb->size());
}

void InputMethodWidget::pause()
{
    emit pauseStreaming();
}

void InputMethodWidget::play()
{
    emit startStreaming();
}

void InputMethodWidget::comboBox(QString item)
{
    if (item == "Camera"){
        browse_video->setVisible(false);
        pause_video->setVisible(true);
        start_video->setVisible(true);
        pause_video->setEnabled(true);
        start_video->setEnabled(true);
        emit changeVideoSource("/dev/video0");
    }
    else if (item == "Video File"){
        browse_video->setVisible(true);
        pause_video->setVisible(true);
        start_video->setVisible(true);
        pause_video->setEnabled(false);
        start_video->setEnabled(false);
        emit reInit();
    }
    else if (item == "None"){
        browse_video->setVisible(false);
        pause_video->setVisible(false);
        start_video->setVisible(false);
        emit reInit();
    }
}

void InputMethodWidget::browseVideo()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Choose video file", "/home", "*.mp4");
    qDebug() << file_name;
    if (file_name.length() == 0)
        return;
    emit changeVideoSource(file_name);
    pause_video->setEnabled(true);
    start_video->setEnabled(true);
    QRegularExpressionMatch chosen_video_match = chosen_video_re.match(file_name);
    QString video;
    if (chosen_video_match.hasMatch())
    {
        video = chosen_video_match.captured(0);
        qDebug() << "[SIDEBAR] video matched" << video;
        emit setVideoName(video);
    }
    else
        qDebug() << "[SIDEBAR] no video matched";
}