#include "stream.h"

Stream::Stream() : width(0), height(0), fps(0)
{
    fsFrame = Q_NULLPTR;
    normal_default_screen = Mat::zeros(480, 854, CV_8UC3);
    ipm_default_screen = Mat::zeros(854, 480, CV_8UC3);
    updateDataLock = true;
    updateFrame = false;
    colorRange = {0, 0, 255};
}

void Stream::changeStreamInSource(QString source)
{
    reInitStream();
    streamInSource = source;
    if (!cap.open(source.toStdString()))
    {
        qDebug() << "[STREAM] Cannot open video streaming source";
        exit(1);
    }
    fps = static_cast<int> (cap.get(cv::CAP_PROP_FPS));
    qDebug() << "[STREAM] using framerate:" << fps;
}

void Stream::reInitStream()
{
    if (cap.isOpened())
        cap.release();
    updateFrame = false;
    initScreens();
}

void Stream::run()
{
    while (true)
    {
        if (!updateFrame)
            continue;
        cap >> inputFrame;
        if (!cap.grab())
        {
            reInitStream();
            continue;
        }
        frames[MultiVideo::normal_rgb] = inputFrame.clone();
        frames[MultiVideo::final_rgb] = inputFrame.clone();
        if (!updateDataLock)
        {
            drawFinalRGB();
        }
        if (!frames[MultiVideo::normal_rgb].empty())
            multiViewer->getVideoWidget(MultiVideo::normal_rgb)->showImage(frames[0]);
        if (!frames[MultiVideo::final_rgb].empty())
            multiViewer->getVideoWidget(MultiVideo::final_rgb)->showImage(frames[1]);
        if (!frames[MultiVideo::ipm_rgb].empty())
            multiViewer->getVideoWidget(MultiVideo::ipm_rgb)->showImage(frames[2]);
        if (!frames[MultiVideo::ipm_bw].empty())
            multiViewer->getVideoWidget(MultiVideo::ipm_bw)->showImage(frames[3]);
        if (fsFrame != Q_NULLPTR)
            fsViewer->getVideoWidget()->showImage(*fsFrame);
        msleep(1350/fps);

//        if ((waitKey(1000/fps)  0xFF) == (int)'q')
//            break;
    }
}

//void Stream::msleep(int ms)
//{
//    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
//    nanosleep(&ts, NULL);
//}

void Stream::stopStream()
{
    qDebug() << "[STREAM] stop streaming";
    updateFrame = false;
}

void Stream::pauseStream()
{
    qDebug() << "[STREAM] pause streaming";
    updateFrame = false;
}

void Stream::startStream()
{
    qDebug() << "[STREAM] start streaming";
    updateFrame = true;
}

void Stream::setViewers(MultiVideoViewer *m, fullScreenVideoViewer *f)
{
    multiViewer = m;
    fsViewer = f;
    connect(multiViewer->getVideoWidget(0), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(1), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(2), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    connect(multiViewer->getVideoWidget(3), SIGNAL(mouseClicked(int)), this, SLOT(FullScreenFrame(int)));
    initScreens();
}

void Stream::setIPMFrame(Mat *f)
{
    frames[MultiVideo::ipm_rgb] = f->clone();
//    frames[MultiVideo::ipm_bw] = f->clone();
}

void Stream::setIPMBW(Mat *f)
{
//    frames[MultiVideo::ipm_rgb] = f->clone();
    frames[MultiVideo::ipm_bw] = f->clone();
}

void Stream::FullScreenFrame(int index)
{
    switch(index)
    {
    case 0:
    case 1:
        fsViewer->setGeometry(0, 0, 854, 480);
        fsViewer->setVideoSize(854, 480);
        break;
    case 2:
    case 3:
        fsViewer->setGeometry(292, 0, 270, 480);
        fsViewer->setVideoSize(270, 480);
        break;
    }
    fsFrame = &(frames[index]);
    fsViewer->getVideoWidget()->showImage(*fsFrame);
}

void Stream::setPointsToDraw(vector<Vec2f> leftPoints, vector<Vec2f> rightPoints)
{
    updateDataLock = true;
    this->leftPts.clear();
    this->rightPts.clear();
    for (unsigned int i = 0; i < leftPoints.size(); i++)
        this->leftPts.push_back(Vec2i{(int)leftPoints[i][0], (int)leftPoints[i][1]});
    for (unsigned int i = 0; i < rightPoints.size(); i++)
        this->rightPts.push_back(Vec2i{(int)rightPoints[i][0], (int)rightPoints[i][1]});
    updateDataLock = false;
}

void Stream::drawFinalRGB()
{
    Mat processed = frames[MultiVideo::final_rgb].clone();
    Mat mask;
    vector<vector<Point>> contours;
    topPts.clear();
    bottomPts.clear();
    topPts.push_back(rightPts.at(0));
    bottomPts.push_back(rightPts.at(rightPts.size()-1));
    topPts.push_back(leftPts.at(0));
    bottomPts.push_back(leftPts.at(leftPts.size()-1));
    polylines(processed, topPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, bottomPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, leftPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, rightPts, false, Scalar(0, 0, 255), 2, LINE_8);
    inRange(processed, colorRange, colorRange, mask);
    findContours(mask, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    if (contours.empty())
        contours = prevContours;
    if (contours.size())
    {
        fillConvexPoly(processed, contours[0], Scalar(0, 255, 0), LINE_AA);
        addWeighted(processed, 0.5, frames[MultiVideo::final_rgb], 1 - 0.5, 0, frames[MultiVideo::final_rgb]);
        prevContours = contours;
    }
}

Mat Stream::getFrame()
{
    return frames[MultiVideo::normal_rgb].clone();
}

void Stream::initScreens()
{
    frames[MultiVideo::normal_rgb] = normal_default_screen;
    frames[MultiVideo::final_rgb] = normal_default_screen;
    frames[MultiVideo::ipm_rgb] = ipm_default_screen;
    frames[MultiVideo::ipm_bw] = ipm_default_screen;
    multiViewer->getVideoWidget(MultiVideo::normal_rgb)->showImage(normal_default_screen);
    multiViewer->getVideoWidget(MultiVideo::final_rgb)->showImage(normal_default_screen);
    multiViewer->getVideoWidget(MultiVideo::ipm_rgb)->showImage(ipm_default_screen);
    multiViewer->getVideoWidget(MultiVideo::ipm_bw)->showImage(ipm_default_screen);
}

Stream::~Stream()
{
    exit(0);
}
