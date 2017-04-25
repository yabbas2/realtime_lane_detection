#include "ipm.h"

void IPM::transform(Mat &original_frame, QString video_name){
    Mat dst_pts, input_pts;
    int height = original_frame.rows;
    int width = original_frame.cols;

    dst_pts = (Mat_<double>(4,2) << 0, 0, height, 0, height, width, 0, width);

    if (video_name == "sample1")
        input_pts = (Mat_<double>(4, 2) << 357, 280, 528, 282, 778, 478, 146, 478);
    else if (video_name == "sample2")
        input_pts = (Mat_<double>(4, 2) << 250, 222, 370, 222, 440, 290, 214, 290);
    else if (video_name == "sample3")
        input_pts = (Mat_<double>(4, 2) << 250, 222, 370, 222, 440, 290, 214, 290);
    else if (video_name == "sample4")
        input_pts = (Mat_<double>(4, 2) << 250, 222, 370, 222, 440, 290, 214, 290);
    else if (video_name == "sample5")
        input_pts = (Mat_<double>(4, 2) << 260, 196, 354, 196, 442, 280, 204, 280);
    else if (video_name == "sample6")
        input_pts = (Mat_<double>(4, 2) << 350, 270, 560, 270, 700, 385, 260, 385);
    else
        qDebug() << ("[IPM] unknown video file!");

    Mat transform_homography = getPerspectiveTransform(input_pts, dst_pts);
    inverse_homography = getPerspectiveTransform(dst_pts, input_pts);
    warpPerspective(original_frame, ipm_frame, transform_homography, Size(height, width));
}

void IPM::inverseTransform(std::vector<Vec2i> &old_pts, std::vector<Vec2i> &new_pts){
    new_pts.clear();
    float z;
    float ptx, pty;
    for(std::vector<Vec2i>::iterator it = old_pts.begin(); it != old_pts.end(); it++){
        z = 1 / (inverse_homography.at<double>(2,0) * (*it)[0] + inverse_homography.at<double>(2,1) * (*it)[1] + inverse_homography.at<double>(2,2));
        ptx =  ((inverse_homography.at<double>(0,0) * (*it)[0] + inverse_homography.at<double>(0,1) * (*it)[1] + inverse_homography.at<double>(0,2)) * z);
        pty =  ((inverse_homography.at<double>(1,0) * (*it)[0] + inverse_homography.at<double>(1,1) * (*it)[1] + inverse_homography.at<double>(1,2)) * z);
        new_pts.push_back(Vec2i{static_cast<int> (ptx), static_cast<int> (pty)});
    }
    assert(new_pts.size() == old_pts.size());
}

Mat *IPM::getIPMFrame()
{
    return &ipm_frame;
}
