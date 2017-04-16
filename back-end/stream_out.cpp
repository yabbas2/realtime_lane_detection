#include "stream_out.h"


StreamOut::StreamOut()
{

}

void StreamOut::drawFinalRGB(cv::Mat *frame)
{

}

void StreamOut::drawFinalBW(cv::Mat *frame)
{

}

void StreamOut::drawIpmRGB(cv::Mat *frame)
{

}

void StreamOut::drawIpmBW(cv::Mat *frame)
{

}

void StreamOut::setDrawingData(std::vector<cv::Vec2i> *lp)
{
    linePoints = *lp;
}
