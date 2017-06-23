#ifndef LINEDETECTION_H
#define LINEDETECTION_H

#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace LineDetector {
    enum {left, right};
}

class LineDetection
{
public:
    LineDetection();
    void lineSegmentDetector(Mat &input_frame);
    void cannyHough(Mat &inputFrame, int side);
    vector<Vec4f> *getDetectedLines();
    vector<Vec4f> *getTmpLines(int side);

private:
    vector<Vec4f> lines;
    vector<Vec4f> tmpLeftLines;
    vector<Vec4f> tmpRightLines;
};

#endif // LINEDETECTION_H
