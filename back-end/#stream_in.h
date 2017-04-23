#ifndef STREAM_IN_H
#define STREAM_IN_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QString>
#include <QTimer>

class StreamIn : public QObject
{
    Q_OBJECT

public:
    explicit StreamIn(QObject *parent = 0);
    ~StreamIn();
    void initStreamIn(QString source);
    cv::Mat *getFrame();
    void getVideoInfo(int &w, int &h, int &fps);

signals:
    void endStream();

private slots:
    void startStreamIn();
    void stopStreamIn();

private:
    cv::Mat inputFrame;
    cv::VideoCapture cap;
    QTimer timer;

};

#endif /* STREAM_IN_H */
