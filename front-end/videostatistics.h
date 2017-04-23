#ifndef VIDEOSTATISTICS_H
#define VIDEOSTATISTICS_H

#include <QWidget>
#include <QApplication>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QGroupBox>
#include <QGraphicsDropShadowEffect>

class VideoStatistics : public QWidget
{
    Q_OBJECT

public:
    VideoStatistics(QWidget *parent = 0);
    ~VideoStatistics();

    void DisplayVideoStatistics();

private:
    QVBoxLayout *verticalLayout_statistics;

    QLabel *ExecTimePerFrameLabel;
    QLabel *ExecTimePerFrameValue;
    QHBoxLayout *ExecTimePerFrame;

    QLabel *AvgExecTimePerFrameLabel;
    QLabel *AvgExecTimePerFrameValue;
    QHBoxLayout *AvgExecTimePerFrame;

    QLabel *AvgFramePerSecLabel;
    QLabel *AvgFramePerSecValue;
    QHBoxLayout *AvgFramePerSec;

    //QLabel *VideoDurationLabel;
    //QProgressBar *VideoDurationProgress;

    QGroupBox *gb_statistics;
    QGraphicsDropShadowEffect *textEffect_statistics;
    QVBoxLayout *mainLayout_statistics;

private slots:
    void setExecTimePerFrameValue(QString value);
    void setAvgExecTimePerFrameValue(QString value);
    void setAvgFramePerSecValue(QString value);
};

#endif // VIDEOSTATISTICS_H
