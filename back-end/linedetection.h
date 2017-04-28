#ifndef LINEDETECTION_H
#define LINEDETECTION_H

#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;


class LineDetection
{
public:
    vector<Vec4f> *lineSegmentDetector(Mat& input_frame);

private:
    vector<Vec4f> lines;

};

#endif // LINEDETECTION_H
