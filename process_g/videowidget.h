#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QString>
#include <QPushButton>
#include <QIcon>
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

private slots:
    void playVideo();
    void pauseVideo();
    void browseVideo();
    void cameraVideo();

signals:
    void pauseStreaming();
    void startStreaming();
    void changeVideoSource(QString);
    void setVideoName(QString);

private:
    QVBoxLayout *mainLayout;

    QPushButton *browseButton;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *cameraButton;

    QGroupBox *gb_controls;
    QHBoxLayout *horizontalLayout_controls;

    QSpacerItem *vSpacer;

    QRegularExpression regexVideo;

    void videoControlInit();
};

#endif // VIDEOWIDGET_H
