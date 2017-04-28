#include "linedetection.h"


vector<Vec4f> *LineDetection::lineSegmentDetector(Mat input_frame)
{
    Mat out;
    cvtColor(input_frame, out, COLOR_BGR2GRAY);
    GaussianBlur(out, out, Size(21, 21), 0, 0);
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector(LSD_REFINE_STD);
    lsd->detect(out, lines);
    return &lines;
}
