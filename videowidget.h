#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QDebug>
#include <QSpacerItem>
#include <QStringList>
#include <QFileDialog>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = 0);

public slots:
    void updateValues(QStringList data);
    void playVideo();
    void pauseVideo();
    void browseVideo();

signals:
    void pauseStreaming();
    void startStreaming();
    void changeVideoSource(QString);
    void setVideoName(QString);

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

    QPushButton *BrowseButton;
    QPushButton *PlayButton;
    QPushButton *PauseButton;

    QGroupBox *gb_controls;
    QHBoxLayout *horizontalLayout_controls;

    QSpacerItem *vSpacer;

    QRegularExpression chosen_video_re;

    void videoInfoInit();
    void videoControlInit();
};

#endif // VIDEOWIDGET_H
