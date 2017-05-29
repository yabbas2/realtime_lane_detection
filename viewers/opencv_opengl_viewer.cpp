#include "opencv_opengl_viewer.h"

CVGLViewer::CVGLViewer( int index, QWidget *parent, int width, int height) :
    QOpenGLWidget(parent)
{
    mBgColor = QColor::fromRgb(235, 235, 235);
    this->setFixedSize(width, height);
    this->index = index;
//    this->setStyleSheet("background: rgba(0, 0, 0, 0); border: 3px solid black; border-radius: 10px;");
}

void CVGLViewer::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();
    float r = ((float)mBgColor.red())/255.0f;
    float g = ((float)mBgColor.green())/255.0f;
    float b = ((float)mBgColor.blue())/255.0f;
    glClearColor(r,g,b,1.0f);
}

void CVGLViewer::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, -height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    recalculatePosition();
    emit imageSizeChanged(mRenderWidth, mRenderHeight);
    updateScene();
}

void CVGLViewer::updateScene()
{
    if (this->isVisible()) update();
}

void CVGLViewer::paintGL()
{
    makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderImage();
}

void CVGLViewer::renderImage()
{
    makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT);
    if (!mRenderQtImg.isNull())
    {
        glLoadIdentity();
        glPushMatrix();
        if (mResizedImg.width() <= 0)
        {
            if (mRenderWidth == mRenderQtImg.width() && mRenderHeight == mRenderQtImg.height())
                mResizedImg = mRenderQtImg;
            else
                mResizedImg = mRenderQtImg.scaled(QSize(mRenderWidth, mRenderHeight), Qt::IgnoreAspectRatio,
                                                                                        Qt::SmoothTransformation);
        }
        glRasterPos2i(mRenderPosX, mRenderPosY);
        glPixelZoom(1, -1);
        glDrawPixels(mResizedImg.width(), mResizedImg.height(), GL_RGBA, GL_UNSIGNED_BYTE, mResizedImg.bits());
        glPopMatrix();
        glFlush();
    }
}

void CVGLViewer::recalculatePosition()
{
    mImgRatio = (float)mOrigImage.cols/(float)mOrigImage.rows;
    mRenderWidth = this->size().width();
    mRenderHeight = floor(mRenderWidth / mImgRatio);
    if (mRenderHeight > this->size().height())
    {
        mRenderHeight = this->size().height();
        mRenderWidth = floor(mRenderHeight * mImgRatio);
    }
    mRenderPosX = floor((this->size().width() - mRenderWidth) / 2);
    mRenderPosY = -floor((this->size().height() - mRenderHeight) / 2);
    mResizedImg = QImage();
}

bool CVGLViewer::showImage(const cv::Mat& image)
{
    if (image.channels() == 3)
        cv::cvtColor(image, mOrigImage, CV_BGR2RGBA);
    else if (image.channels() == 1)
        cv::cvtColor(image, mOrigImage, CV_GRAY2RGBA);
    else return false;
    mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data), mOrigImage.cols, mOrigImage.rows,
                                                                    mOrigImage.step1(), QImage::Format_RGB32);
    recalculatePosition();
    updateScene();
    return true;
}

void CVGLViewer::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "[OPENGL] clicked on video:" << this->index;
    if (event->button() == Qt::MouseButton::LeftButton)
        emit mouseClicked(this->index);
}
