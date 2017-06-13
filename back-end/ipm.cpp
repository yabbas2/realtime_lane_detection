#include "ipm.h"

void IPM::transform(Mat &original_frame, QString video_name){
    Mat dst_pts, input_pts;
    int height = original_frame.rows;
    int width = original_frame.cols;

    dst_pts = (Mat_<double>(4,2) << 0, 0, height, 0, height, width, 0, width);

    if (video_name == "youtube_video1")
//        input_pts = (Mat_<double>(4, 2) << 357, 280, 528, 282, 778, 478, 146, 478);
        input_pts = (Mat_<double>(4, 2) << 325, 280, 485, 280, 748, 478, 120, 478);
    else if (video_name.startsWith("kitti_video"))
        input_pts = (Mat_<double>(4, 2) << 330, 300, 460, 300, 600, 478, 200, 478);
    else if (video_name == "sample3")
        input_pts = (Mat_<double>(4, 2) << 250, 222, 370, 222, 440, 290, 214, 290);
    else if (video_name == "sample4")
        input_pts = (Mat_<double>(4, 2) << 250, 222, 370, 222, 440, 290, 214, 290);
    else if (video_name == "sample5")
        input_pts = (Mat_<double>(4, 2) << 260, 196, 354, 196, 442, 280, 204, 280);
    else if (video_name == "youtube_video2")
//        input_pts = (Mat_<double>(4, 2) << 350, 270, 560, 270, 700, 385, 260, 385);
        input_pts = (Mat_<double>(4, 2) << 340, 280, 520, 280, 770, 478, 120, 478);
    else
        qDebug() << ("[IPM] unknown video file!");

    input_pts.convertTo(input_pts, CV_32F);
    dst_pts.convertTo(dst_pts, CV_32F);

    Mat transform_homography = getPerspectiveTransform(input_pts, dst_pts);
    inverse_homography = getPerspectiveTransform(dst_pts, input_pts);
    warpPerspective(original_frame, ipm_frame, transform_homography, Size(height, width));
}

void IPM::inverseTransform(vector<Vec2f> &pts, char c){
    vector<Vec2f>* oldPoints ;
    if (c == ipm::left)
        oldPoints = &leftPoints;
    else if (c == ipm::right)
        oldPoints = &rightPoints;
    if(pts.empty())
    {
        oldPoints->clear();
        return;
    }
    float z;
    float ptx, pty;
    oldPoints->clear();
    for(unsigned int it = 0; it < pts.size(); it++){
        z = 1 / (inverse_homography.at<double>(2,0) * pts[it][0] + inverse_homography.at<double>(2,1) * pts[it][1] + inverse_homography.at<double>(2,2));
        ptx =  ((inverse_homography.at<double>(0,0) * pts[it][0] + inverse_homography.at<double>(0,1) * pts[it][1] + inverse_homography.at<double>(0,2)) * z);
        pty =  ((inverse_homography.at<double>(1,0) * pts[it][0] + inverse_homography.at<double>(1,1) * pts[it][1] + inverse_homography.at<double>(1,2)) * z);
        oldPoints->push_back(Vec2f{ptx, pty});
    }
}

Mat *IPM::getIPMFrame()
{
    return &ipm_frame;
}

vector<Vec2f> *IPM::getPoints(int side)
{
    switch(side)
    {
        case ipm::left:
        return &leftPoints;
        case ipm::right:
        return &rightPoints;
    }
}
