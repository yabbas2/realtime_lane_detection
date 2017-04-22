#include "videostatistics.h"

VideoStatistics::VideoStatistics(QWidget *parent) : QWidget(parent)
{
    this->setFixedWidth(250);
    this->setFixedHeight(150);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    DisplayVideoStatistics();
}

VideoStatistics::~VideoStatistics()
{

}

void VideoStatistics::DisplayVideoStatistics()
{
    gb_statistics = new QGroupBox("Video Statistics", this);
    qApp->setStyleSheet("QGroupBox {border: 2px solid white; font: bold;} QGroupBox::title {padding-top: -7px;}");
    gb_statistics->setAlignment(Qt::AlignHCenter);

    textEffect_statistics = new QGraphicsDropShadowEffect;
    textEffect_statistics->setColor(QColor("#000000"));
    textEffect_statistics->setBlurRadius(10);
    textEffect_statistics->setOffset(0.5, 0.5);
    gb_statistics->setGraphicsEffect(textEffect_statistics);
    gb_statistics->setStyleSheet("color: white; font: bold;");
    //
    //Execution Time Per Frame//
    //
    ExecTimePerFrameLabel = new QLabel(gb_statistics);
    ExecTimePerFrameLabel->setText("Execution Time:");

    ExecTimePerFrameValue = new QLabel(gb_statistics);
    ExecTimePerFrameValue->setText("0.00 msec/frame");
    ExecTimePerFrameValue->setAlignment(Qt::AlignCenter);

    ExecTimePerFrame = new QHBoxLayout;
    ExecTimePerFrame->addWidget(ExecTimePerFrameLabel);
    ExecTimePerFrame->addWidget(ExecTimePerFrameValue);
    //
    //Average Execution Time Per Second//
    //
    AvgExecTimePerFrameLabel = new QLabel(gb_statistics);
    AvgExecTimePerFrameLabel->setText("Avg. Exec Time:");

    AvgExecTimePerFrameValue = new QLabel(gb_statistics);
    AvgExecTimePerFrameValue->setText("0.00 msec/frame");
    AvgExecTimePerFrameValue->setAlignment(Qt::AlignCenter);

    AvgExecTimePerFrame = new QHBoxLayout;
    AvgExecTimePerFrame->addWidget(AvgExecTimePerFrameLabel);
    AvgExecTimePerFrame->addWidget(AvgExecTimePerFrameValue);
    //
    //Average Frame Per Second//
    //
    AvgFramePerSecLabel = new QLabel(gb_statistics);
    AvgFramePerSecLabel->setText("AFPS:");

    AvgFramePerSecValue = new QLabel(gb_statistics);
    AvgFramePerSecValue->setText("0 frame/sec");
    AvgFramePerSecValue->setAlignment(Qt::AlignCenter);

    AvgFramePerSec = new QHBoxLayout;
    AvgFramePerSec->addWidget(AvgFramePerSecLabel);
    AvgFramePerSec->addWidget(AvgFramePerSecValue);
    //
    //Video Progress Bar//
    //
    //VideoDurationLabel = new QLabel(gb);
    //VideoDurationLabel->setText("Video Duration %");
    //VideoDurationProgress = new QProgressBar(gb);
    //VideoDurationProgress->setValue(25);
    //
    //aligning all horizontal layouts and widgets vertically//
    //
    verticalLayout_statistics = new QVBoxLayout;
    verticalLayout_statistics->addLayout(ExecTimePerFrame);
    verticalLayout_statistics->addLayout(AvgExecTimePerFrame);
    verticalLayout_statistics->addLayout(AvgFramePerSec);
    //verticalLayout_statistics->addWidget(VideoDurationLabel);
    //verticalLayout_statistics->addWidget(VideoDurationProgress);
    gb_statistics->setLayout(verticalLayout_statistics);
    mainLayout_statistics = new QVBoxLayout;
    mainLayout_statistics->addWidget(gb_statistics);
    this->setLayout(mainLayout_statistics);
}

void VideoStatistics::setExecTimePerFrameValue(QString value)
{
    ExecTimePerFrameValue->setText(value + " msec/frame");
}

void VideoStatistics::setAvgExecTimePerFrameValue(QString value)
{
    ExecTimePerFrameValue->setText(value + " msec/frame");
}

void VideoStatistics::setAvgFramePerSecValue(QString value)
{
    ExecTimePerFrameValue->setText(value + " frame/sec");
}
