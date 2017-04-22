#include "videocontrols.h"

VideoControls::VideoControls(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedWidth(200);
    this->setFixedHeight(90);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    DisplayVideoControls();
}

VideoControls::~VideoControls()
{

}

void VideoControls::DisplayVideoControls()
{
    gb_controls = new QGroupBox("Controls", this);
    qApp->setStyleSheet("QGroupBox {border: 2px solid white; font: bold;} QGroupBox::title {padding-top: -7px;}");
    gb_controls->setAlignment(Qt::AlignHCenter);

    textEffect_controls = new QGraphicsDropShadowEffect;
    textEffect_controls->setColor(QColor("#000000"));
    textEffect_controls->setBlurRadius(10);
    textEffect_controls->setOffset(0.5, 0.5);
    gb_controls->setGraphicsEffect(textEffect_controls);
    gb_controls->setStyleSheet("color: white; font: bold;");
    //
    //Back Button//
    //
    BackButton = new QToolButton(gb_controls);
    BackButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    BackButton->setText("Back");
    BackButton->setStyleSheet("color: black;");
    BackButton->setFixedWidth(50);
    QPixmap BackLogo("C:/Users/Yousif/Documents/QtProjects/VideoWidgets/back.png");
    QIcon BackIcon(BackLogo);
    BackButton->setIcon(BackIcon);
    //
    //Play Button//
    //
    PlayButton = new QToolButton(gb_controls);
    PlayButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    PlayButton->setText("Play");
    PlayButton->setStyleSheet("color: black;");
    PlayButton->setFixedWidth(50);
    QPixmap PlayLogo("C:/Users/Yousif/Documents/QtProjects/VideoWidgets/play.png");
    QIcon PlayIcon(PlayLogo);
    PlayButton->setIcon(PlayIcon);
    //
    //Pause Button//
    //
    PauseButton = new QToolButton(gb_controls);
    PauseButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    PauseButton->setText("Pause");
    PauseButton->setStyleSheet("color: black;");
    PauseButton->setFixedWidth(50);
    QPixmap PauseLogo("C:/Users/Yousif/Documents/QtProjects/VideoWidgets/pause.png");
    QIcon PauseIcon(PauseLogo);
    PauseButton->setIcon(PauseIcon);
    //
    //aligining all buttons horizontally//
    //
    horizontalLayout_controls = new QHBoxLayout;
    horizontalLayout_controls->addWidget(BackButton);
    horizontalLayout_controls->addWidget(PlayButton);
    horizontalLayout_controls->addWidget(PauseButton);
    gb_controls->setLayout(horizontalLayout_controls);
    mainLayout_controls = new QHBoxLayout;
    mainLayout_controls->addWidget(gb_controls);
    this->setLayout(mainLayout_controls);
}
