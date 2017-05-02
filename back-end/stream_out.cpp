#include "stream_out.h"


StreamOut::StreamOut()
{
    colorRange = {0, 0, 255};
}

void StreamOut::drawFinalRGB(Mat &frame)
{
    Mat processed = frame.clone();
//    Mat mask;
//    vector<vector<Point>> contours;
//    topPts.clear();
//    bottomPts.clear();
//    topPts.push_back(rightPts->at(0));
//    bottomPts.push_back(rightPts->at(0));
//    topPts.push_back(leftPts->at(0));
//    bottomPts.push_back(leftPts->at(0));
//    polylines(processed, topPts, false, Scalar(255, 127, 127), 2, LINE_8);
//    polylines(processed, bottomPts, false, Scalar(255, 127, 127), 2, LINE_8);
    polylines(processed, *leftPts, false, Scalar(0, 0, 255), 2, LINE_8);
    polylines(processed, *rightPts, false, Scalar(0, 0, 255), 2, LINE_8);
//    inRange(processed, colorRange, colorRange, mask);
//    findContours(mask, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
//    if (contours.empty())
//        contours = prevContours;
//    if (contours.size())
//    {
//        fillConvexPoly(processed, contours[0], Scalar(0, 255, 0), LINE_AA);
//        addWeighted(processed, 0.5, frame, 1 - 0.5, 0, frame);
//        prevContours = contours;
//    }
    frame = processed.clone();
}

void StreamOut::drawIpmRGB(Mat &frame)
{

}

void StreamOut::drawIpmBW(Mat &frame)
{

}

void StreamOut::setDrawingData(vector<Vec2i> *leftPts, vector<Vec2i> *rightPts)
{
    this->leftPts = leftPts;
    this->rightPts = rightPts;
}
