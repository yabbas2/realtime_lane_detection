#ifndef STREAM_H
#define STREAM_H

#include <QString>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <QtDBus>
#include <QApplication>

#define StreamingVideos     4

namespace stream {
enum {normal_rgb, final_rgb, ipm_rgb, ipm_bw};
}

using namespace cv;
using namespace std;

class Stream : public QApplication
{
    Q_OBJECT

public:
    explicit Stream(int argc, char *argv[]);
    ~Stream();
    void setInfo(vector<Vec2f> leftPts, vector<Vec2f> rightPts, Vec2i leftProp, Vec2i rightProp);
    Mat getFrame();
    void setIPMFrame(Mat *f);
    void setIPMBW(Mat *f);
    void reInitStream();
    int width, height, fps;

public slots:
    void changeStreamInSource(QString source);
    void initScreens();
    void pauseStream();
    void startStream();

private slots:
    void showFrames();
    void FullScreenFrame(int index);

private:
    QString streamInSource;
    cv::Mat inputFrame;
    cv::VideoCapture cap;
    Mat frames[StreamingVideos];
    Mat *fsFrame;
    Scalar leftColor;
    Scalar rightColor;
    Mat normal_default_screen;
    Mat ipm_default_screen;
    vector<Vec2i> leftPts;
    vector<Vec2i> rightPts;
    bool updateDataLock;
    vector<Vec2i> bottomPts;
    vector<Vec2i> topPts;
    vector<vector<Point>> prevContours;
    vector<int> colorRange;
    QTimer *timer;
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *ifGUI;

    void drawFinalRGB();

};

#endif // STREAM_H
