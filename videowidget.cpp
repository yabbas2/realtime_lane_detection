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
    PlayButton->setEnabled(false);
    PauseButton->setEnabled(false);

    chosen_video_re.setPattern("[youtubekittiudacity]+_video[0-9]+");

    connect(PlayButton, SIGNAL(clicked(bool)), this, SLOT(playVideo()));
    connect(PauseButton, SIGNAL(clicked(bool)), this, SLOT(pauseVideo()));
    connect(BrowseButton, SIGNAL(clicked(bool)), this, SLOT(browseVideo()));
}

void VideoWidget::playVideo()
{
    emit startStreaming();
}

void VideoWidget::pauseVideo()
{
    emit pauseStreaming();
}

void VideoWidget::browseVideo()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Choose video file", "/home", "*.mp4");
    if (file_name.length() == 0)
        return;
    emit changeVideoSource(file_name);
    PlayButton->setEnabled(true);
    PauseButton->setEnabled(true);
    QRegularExpressionMatch chosen_video_match = chosen_video_re.match(file_name);
    QString video;
    if (chosen_video_match.hasMatch())
    {
        video = chosen_video_match.captured(0);
        qDebug() << "[VIDEO_WIDGET] video matched" << video;
        emit setVideoName(video);
    }
    else
        qDebug() << "[VIDEO_WIDGET] no video matched";
}

void VideoWidget::videoControlInit()
{
    gb_controls = new QGroupBox(this);
    gb_controls->setStyleSheet("QGroupBox {border: 1px solid white; border-radius: 5px;}");
    gb_controls->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    BrowseButton = new QPushButton(gb_controls);
    QIcon backIcon(":/icons/resources/buttons/browse.png");
    BrowseButton->setIcon(backIcon);
    BrowseButton->setFixedWidth(50);

    PlayButton = new QPushButton(gb_controls);
    QIcon playIcon(":/icons/resources/buttons/play.png");
    PlayButton->setIcon(playIcon);
    PlayButton->setFixedWidth(50);

    PauseButton = new QPushButton(gb_controls);
    QIcon pauseIcon(":/icons/resources/buttons/pause.png");
    PauseButton->setIcon(pauseIcon);
    PauseButton->setFixedWidth(50);

    horizontalLayout_controls = new QHBoxLayout;
    horizontalLayout_controls->addWidget(BrowseButton);
    horizontalLayout_controls->addWidget(PlayButton);
    horizontalLayout_controls->addWidget(PauseButton);
    gb_controls->setLayout(horizontalLayout_controls);
    gb_controls->setFixedSize(200, 40);
}
