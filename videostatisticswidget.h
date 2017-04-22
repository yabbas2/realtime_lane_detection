#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QGroupBox>
#include <QGraphicsDropShadowEffect>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void VideoStatistics();

private:
    Ui::Widget *ui;
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

#endif // WIDGET_H
