#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent)
{
    videoInfoInit();
    videoControlInit();
    mainLayout = new QVBoxLayout;
    vSpacer = new QSpacerItem(0, 320);
    mainLayout->addWidget(gb_statistics);
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

void VideoWidget::updateValues(QStringList data)
{
    ExecTimePerFrameValue->setText(data.at(0) + " msec/frame");
    AvgExecTimePerFrameValue->setText(data.at(1) + " msec/frame");
    FPS_Value->setText(data.at(2) + " frame/sec");
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

void VideoWidget::videoInfoInit()
{
    gb_statistics = new QGroupBox(this);
    gb_statistics->setStyleSheet("QGroupBox {border: 1px solid white; border-radius: 5px;}");
    gb_statistics->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    ExecTimePerFrameLabel = new QLabel(gb_statistics);
    ExecTimePerFrameLabel->setText("Exec. Time:");
    ExecTimePerFrameLabel->setStyleSheet("font: bold; color: white;");

    ExecTimePerFrameValue = new QLabel(gb_statistics);
    ExecTimePerFrameValue->setText("000.00 msec/frame");
    ExecTimePerFrameValue->setStyleSheet("font: bold; color: white;");
    ExecTimePerFrameValue->setFixedWidth(130);

    ExecTimePerFrame = new QHBoxLayout;
    ExecTimePerFrame->addWidget(ExecTimePerFrameLabel);
    ExecTimePerFrame->addWidget(ExecTimePerFrameValue);

    AvgExecTimePerFrameLabel = new QLabel(gb_statistics);
    AvgExecTimePerFrameLabel->setText("Avg. Exec. Time:");
    AvgExecTimePerFrameLabel->setStyleSheet("font: bold; color: white;");

    AvgExecTimePerFrameValue = new QLabel(gb_statistics);
    AvgExecTimePerFrameValue->setText("000.00 msec/frame");
    AvgExecTimePerFrameValue->setStyleSheet("font: bold; color: white;");
    AvgExecTimePerFrameValue->setFixedWidth(130);

    AvgExecTimePerFrame = new QHBoxLayout;
    AvgExecTimePerFrame->addWidget(AvgExecTimePerFrameLabel);
    AvgExecTimePerFrame->addWidget(AvgExecTimePerFrameValue);

    FPS_Label = new QLabel(gb_statistics);
    FPS_Label->setText("FPS:");
    FPS_Label->setStyleSheet("font: bold; color: white;");

    FPS_Value = new QLabel(gb_statistics);
    FPS_Value->setText("0 frame/sec");
    FPS_Value->setStyleSheet("font: bold; color: white;");
    FPS_Value->setFixedWidth(130);

    AvgFramePerSec = new QHBoxLayout;
    AvgFramePerSec->addWidget(FPS_Label);
    AvgFramePerSec->addWidget(FPS_Value);

    verticalLayout_statistics = new QVBoxLayout;
    verticalLayout_statistics->addLayout(ExecTimePerFrame);
    verticalLayout_statistics->addLayout(AvgExecTimePerFrame);
    verticalLayout_statistics->addLayout(AvgFramePerSec);

    gb_statistics->setLayout(verticalLayout_statistics);
    gb_statistics->setFixedHeight(80);
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
