#include "linedetection.h"


LineDetection::LineDetection()
{

}

void LineDetection::lineSegmentDetector(Mat &input_frame)
{
    Mat processed;
    cvtColor(input_frame, processed, COLOR_BGR2GRAY);
    GaussianBlur(processed, processed, Size(9, 9), 1, 2);
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector(LSD_REFINE_STD);
    lsd->detect(processed, lines);
}

void LineDetection::cannyHough(Mat &inputFrame, int side)
{
    Mat edgeImage;
    int xThreshold = inputFrame.cols / 2;
    vector<Vec4i> tmpLines;
    Canny(inputFrame, edgeImage, 50, 70);
    HoughLinesP(edgeImage, tmpLines, 1, CV_PI/180, 50, 50, 70);
    if (side == LineDetector::left)
    {
        tmpLeftLines.clear();
        for (size_t i = 0; i < tmpLines.size(); ++i)
            if (tmpLines[i][0] <= xThreshold && tmpLines[i][2] <= xThreshold)
                tmpLeftLines.push_back(Vec4f{(float)tmpLines[i][0], (float)tmpLines[i][1], (float)tmpLines[i][2], (float)tmpLines[i][3]});
    }
    if (side == LineDetector::right)
    {
        tmpRightLines.clear();
        for (size_t i = 0; i < tmpLines.size(); ++i)
            if (tmpLines[i][0] > xThreshold && tmpLines[i][2] > xThreshold)
                tmpRightLines.push_back(Vec4f{(float)tmpLines[i][0], (float)tmpLines[i][1], (float)tmpLines[i][2], (float)tmpLines[i][3]});
    }
}

vector<Vec4f> *LineDetection::getDetectedLines()
{
    return &lines;
}

vector<Vec4f> *LineDetection::getTmpLines(int side)
{
    switch (side)
    {
    case LineDetector::left:
        return &tmpLeftLines;
    case LineDetector::right:
        return &tmpRightLines;
    }
}
