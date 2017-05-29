#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QString>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QIcon>
#include <QSpacerItem>
#include <QFont>
#include <QStringList>

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = 0);

public slots:
    void updateValues(QStringList data);
    void playVideo();
    void pauseVideo();
    void backToMain();

signals:
    void pauseStreaming();
    void startStreaming();
    void switchToMain();

private:
    QVBoxLayout *verticalLayout_statistics;

    QLabel *ExecTimePerFrameLabel;
    QLabel *ExecTimePerFrameValue;
    QHBoxLayout *ExecTimePerFrame;

    QLabel *AvgExecTimePerFrameLabel;
    QLabel *AvgExecTimePerFrameValue;
    QHBoxLayout *AvgExecTimePerFrame;

    QLabel *FPS_Label;
    QLabel *FPS_Value;
    QHBoxLayout *AvgFramePerSec;

    QGroupBox *gb_statistics;
    QVBoxLayout *mainLayout;
    QGraphicsDropShadowEffect *textEffect_statistics;

    QPushButton *BackButton;
    QPushButton *PlayButton;
    QPushButton *PauseButton;

    QGroupBox *gb_controls;
    QGraphicsDropShadowEffect *textEffect_controls;
    QHBoxLayout *horizontalLayout_controls;

    QSpacerItem *vSpacer;

    void videoInfoInit();
    void videoControlInit();
};

#endif // VIDEOWIDGET_H
