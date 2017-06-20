#include "opencv_opengl_viewer.h"

CViewer::CViewer(int index, QWidget *parent, int width, int height) :
    QWidget(parent)
{
    this->setFixedSize(width, height);
    this->index = index;
    viewer = new QLabel(this);
    viewer->setFixedSize(this->size());
}

void CViewer::setSize(int w, int h)
{
    this->setFixedSize(w, h);
    viewer->setFixedSize(this->size());
}

void CViewer::showImage(const cv::Mat &image)
{
    cv::resize(image, mOrigImage, cv::Size(viewer->width(), viewer->height()), 0, 0, cv::INTER_AREA);
    temp = QtOcv::mat2Image(mOrigImage, QtOcv::MCO_BGR, QImage::Format_RGB888);
    mRenderQtImg = QPixmap::fromImage(temp);
    viewer->setPixmap(mRenderQtImg);
}

void CViewer::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "[CVIEWER] clicked on video:" << this->index;
    if (event->button() == Qt::MouseButton::LeftButton)
        emit mouseClicked(this->index);
}
