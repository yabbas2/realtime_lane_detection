#ifndef STREAM_OUT_H
#define STREAM_OUT_H

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class StreamOut
{
public:
    StreamOut();
    void drawFinalRGB(Mat &frame);
    void drawIpmRGB(Mat &frame);
    void drawIpmBW(Mat &frame);
    void setDrawingData(vector<Vec2i> *leftPts, vector<Vec2i> *rightPts);

private:
    vector<Vec2i> *leftPts;
    vector<Vec2i> *rightPts;
    vector<Vec2i> bottomPts;
    vector<Vec2i> topPts;
    vector<vector<Point>> prevContours;
    vector<int> colorRange;
    bool updateDataLock;
};

#endif // STREAM_OUT_H
