#ifndef LINEDETECTION_H
#define LINEDETECTION_H

#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class LineDetection
{
public:
    LineDetection();
    void lineSegmentDetector(Mat &input_frame);
    vector<Vec4f> *getDetectedLines();

private:
    vector<Vec4f> lines;
};

#endif // LINEDETECTION_H
