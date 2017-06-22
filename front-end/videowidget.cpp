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

    connect(PlayButton, SIGNAL(clicked(bool)), this, SLOT(playVideo()));
    connect(PauseButton, SIGNAL(clicked(bool)), this, SLOT(pauseVideo()));
    connect(BackButton, SIGNAL(clicked(bool)), this, SLOT(backToMain()));
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

void VideoWidget::backToMain()
{
    emit switchToMain();
}

void VideoWidget::videoInfoInit()
{
    gb_statistics = new QGroupBox(this);
    gb_statistics->setStyleSheet("QGroupBox {border: 1px solid white; border-radius: 5px;}");
    gb_statistics->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    textEffect_statistics = new QGraphicsDropShadowEffect;
    textEffect_statistics->setColor(QColor("#000000"));
    textEffect_statistics->setBlurRadius(10);
    textEffect_statistics->setOffset(0.5, 0.5);
    gb_statistics->setGraphicsEffect(textEffect_statistics);

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
    textEffect_controls = new QGraphicsDropShadowEffect;
    textEffect_controls->setColor(QColor("#000000"));
    textEffect_controls->setBlurRadius(10);
    textEffect_controls->setOffset(0.5, 0.5);
    gb_controls->setGraphicsEffect(textEffect_controls);

    BackButton = new QPushButton(gb_controls);
    QIcon backIcon(":/icons/resources/buttons/back.png");
    BackButton->setIcon(backIcon);
    BackButton->setFixedWidth(50);

    PlayButton = new QPushButton(gb_controls);
    QIcon playIcon(":/icons/resources/buttons/play.png");
    PlayButton->setIcon(playIcon);
    PlayButton->setFixedWidth(50);

    PauseButton = new QPushButton(gb_controls);
    QIcon pauseIcon(":/icons/resources/buttons/pause.png");
    PauseButton->setIcon(pauseIcon);
    PauseButton->setFixedWidth(50);

    horizontalLayout_controls = new QHBoxLayout;
    horizontalLayout_controls->addWidget(BackButton);
    horizontalLayout_controls->addWidget(PlayButton);
    horizontalLayout_controls->addWidget(PauseButton);
    gb_controls->setLayout(horizontalLayout_controls);
    gb_controls->setFixedSize(200, 40);
}
