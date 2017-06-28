#include "ipm.h"

IPM::IPM()
{
    dstPts = (Mat_<double>(4,2) << 0, 0, frameHeight, 0, frameHeight, frameWidth, 0, frameWidth);
    dstPts.convertTo(dstPts, CV_32F);
}

void IPM::setVideoPts(QString videoName)
{
    if (videoName == "youtube_video1")
        inputPts = (Mat_<double>(4, 2) << 325, 280, 485, 280, 748, 478, 120, 478);
    else if (videoName == "youtube_video2" || videoName == "youtube_video3")
        inputPts = (Mat_<double>(4, 2) << 340, 280, 520, 280, 770, 478, 120, 478);
    else if (videoName == "youtube_video4")
        inputPts = (Mat_<double>(4, 2) << 310, 280, 445, 280, 615, 390, 200, 390);
    else if (videoName == "youtube_video5")
        inputPts = (Mat_<double>(4, 2) << 365, 300, 490, 300, 700, 440, 210, 440);
    else if (videoName == "youtube_video6")
        inputPts = (Mat_<double>(4, 2) << 380, 220, 480, 220, 610, 350, 235, 350);
    else if (videoName.startsWith("kitti_video"))
        inputPts = (Mat_<double>(4, 2) << 330, 300, 460, 300, 600, 478, 200, 478);
    else if (videoName.startsWith("udacity_video"))
        inputPts = (Mat_<double>(4, 2) << 340, 310, 475, 310, 740, 430, 120, 430);

    inputPts.convertTo(inputPts, CV_32F);
    transformHomography = getPerspectiveTransform(inputPts, dstPts);
    inverseHomography = getPerspectiveTransform(dstPts, inputPts);
}

void IPM::transform(Mat &frame)
{
    warpPerspective(frame, ipmFrame, transformHomography, Size(frameHeight, frameWidth));
}
