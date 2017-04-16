#ifndef STREAMIN_H
#define STREAMIN_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QString>

class StreamIn : public QObject
{
    Q_OBJECT

public:
    explicit StreamIn(QObject *parent = 0);
    ~StreamIn();
    void initStreamIn(QString source, QThread &thread);
    cv::Mat *getFrame();

signals:
    void endStream();
protected slots:
    void startStreamIn();
    void stopStreamIn();

private:
    cv::Mat inputFrame;
    cv::VideoCapture cap;
};

#endif /* STREAMIN_H */
