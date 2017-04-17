#ifndef STREAM_OUT_H
#define STREAM_OUT_H

#include <vector>
#include <opencv2/opencv.hpp>

class StreamOut
{
public:
    StreamOut();
    void drawFinalRGB(cv::Mat *frame);
    void drawIpmRGB(cv::Mat *frame);
    void drawIpmBW(cv::Mat *frame);
    void setDrawingData(std::vector<cv::Vec2i> *lp);

private:
    std::vector<cv::Vec2i> linePoints;

};

#endif // STREAM_OUT_H
