#include "linedetection.h"


LineDetection::LineDetection()
{

}

void LineDetection::lineSegmentDetector(Mat &input_frame)
{
    Mat processed;
    cvtColor(input_frame, processed, COLOR_BGR2GRAY);
    GaussianBlur(processed, processed, Size(21, 21), 0, 0);
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector(LSD_REFINE_STD);
    lines.clear();
    lsd->detect(processed, lines);
}

vector<Vec4f> *LineDetection::getDetectedLines()
{
    return &lines;
}
