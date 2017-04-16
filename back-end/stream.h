#ifndef STREAM_H
#define STREAM_H

#include <opencv2/opencv.hpp>

class Stream
{
public:
    Stream();
    ~Stream();
    template<typename S> void setVideoSource(S source);
    cv::Mat inputFrame;

private:
};

#endif /* STREAM_H */
