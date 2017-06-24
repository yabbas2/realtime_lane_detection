#ifndef OPENCVVIEWER_H
#define OPENCVVIEWER_H

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/photo.hpp>
#include <QMouseEvent>
#include <QDebug>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QWidget>

class CViewer : public QWidget
{
    Q_OBJECT
public:
    explicit CViewer(int index, QWidget *parent = 0, int width = 100, int height = 100);
    void setSize(int w, int h);
    int index;

signals:
    void mouseClicked(int index);

public slots:
    void showImage(const cv::Mat& image);

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    QPixmap mRenderQtImg;
    QImage temp;
    cv::Mat mOrigImage;
    QLabel *viewer;
};

#endif /*OPENCVOPENGLVIEWER_H*/
