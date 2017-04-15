#ifndef OPENCVOPENGLVIEWER_H
#define OPENCVOPENGLVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_0>
#include <opencv2/core/core.hpp>
#include <QMouseEvent>

class CVGLViewer : public QOpenGLWidget, protected QOpenGLFunctions_2_0
{
    Q_OBJECT
public:
    explicit CVGLViewer(QWidget *parent = 0);

signals:
    void imageSizeChanged( int outW, int outH );
    void mouseClicked();

public slots:
    bool showImage(const cv::Mat& image);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void updateScene();
    void renderImage();
    void mousePressEvent(QMouseEvent *event);

private:
    QImage mRenderQtImg;
    QImage mResizedImg;
    cv::Mat mOrigImage;
    QColor mBgColor;
    float mImgRatio;
    int mRenderWidth;
    int mRenderHeight;
    int mRenderPosX;
    int mRenderPosY;

    void recalculatePosition();
};

#endif /*OPENCVOPENGLVIEWER_H*/
