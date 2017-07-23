#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent)
{
    videoControlInit();
    mainLayout = new QVBoxLayout;
    vSpacer = new QSpacerItem(0, 400);
    mainLayout->addSpacerItem(vSpacer);
    mainLayout->addWidget(gb_controls);
    this->setLayout(mainLayout);
    playButton->setEnabled(false);
    pauseButton->setEnabled(false);

    regexVideo.setPattern("[youtubekittiudacity]+_video[0-9]+");

    connect(playButton, SIGNAL(clicked(bool)), this, SLOT(playVideo()));
    connect(pauseButton, SIGNAL(clicked(bool)), this, SLOT(pauseVideo()));
    connect(browseButton, SIGNAL(clicked(bool)), this, SLOT(browseVideo()));
    connect(cameraButton, SIGNAL(clicked(bool)), this, SLOT(cameraVideo()));
}

void VideoWidget::playVideo()
{
    emit startStreaming();
}

void VideoWidget::pauseVideo()
{
    emit pauseStreaming();
}

void VideoWidget::cameraVideo()
{
    emit changeVideoSource("/dev/video0");
    emit setVideoName("video0");
    playButton->setEnabled(true);
    pauseButton->setEnabled(true);
}

void VideoWidget::browseVideo()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Choose video file", "/home", "*");
    if (file_name.length() == 0)
        return;
    emit changeVideoSource(file_name);
    playButton->setEnabled(true);
    pauseButton->setEnabled(true);
    QRegularExpressionMatch chosen_video_match = regexVideo.match(file_name);
    QString video;
    if (chosen_video_match.hasMatch())
    {
        video = chosen_video_match.captured(0);
        emit setVideoName(video);
    }
}

void VideoWidget::videoControlInit()
{
    gb_controls = new QGroupBox(this);
    gb_controls->setStyleSheet("QGroupBox {border: 1px solid white; border-radius: 5px;}");
    gb_controls->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    browseButton = new QPushButton(gb_controls);
    QIcon backIcon(":/icons/resources/buttons/browse.png");
    browseButton->setIcon(backIcon);
    browseButton->setFixedWidth(50);

    playButton = new QPushButton(gb_controls);
    QIcon playIcon(":/icons/resources/buttons/play.png");
    playButton->setIcon(playIcon);
    playButton->setFixedWidth(50);

    pauseButton = new QPushButton(gb_controls);
    QIcon pauseIcon(":/icons/resources/buttons/pause.png");
    pauseButton->setIcon(pauseIcon);
    pauseButton->setFixedWidth(50);

    cameraButton = new QPushButton(gb_controls);
    QIcon cameraIcon(":/icons/resources/buttons/camera.png");
    cameraButton->setIcon(cameraIcon);
    cameraButton->setIconSize(QSize(27, 27));
    cameraButton->setFixedWidth(50);

    horizontalLayout_controls = new QHBoxLayout;
    horizontalLayout_controls->addWidget(browseButton);
    horizontalLayout_controls->addWidget(cameraButton);
    horizontalLayout_controls->addWidget(playButton);
    horizontalLayout_controls->addWidget(pauseButton);
    gb_controls->setLayout(horizontalLayout_controls);
    gb_controls->setFixedSize(240, 40);
}
