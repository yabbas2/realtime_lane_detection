#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QString>
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
    void playVideo();
    void pauseVideo();
    void browseVideo();

signals:
    void pauseStreaming();
    void startStreaming();
    void changeVideoSource(QString);
    void setVideoName(QString);

private:
    QVBoxLayout *mainLayout;

    QPushButton *BrowseButton;
    QPushButton *PlayButton;
    QPushButton *PauseButton;

    QGroupBox *gb_controls;
    QHBoxLayout *horizontalLayout_controls;

    QSpacerItem *vSpacer;

    QRegularExpression chosen_video_re;

    void videoControlInit();
};

#endif // VIDEOWIDGET_H
