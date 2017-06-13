#include "filter.h"

Filter::Filter()
{

}

vector<Vec7i>* Filter::getFilteredLines()
{
    return &lines;
}

Mat Filter::falseDetectionElimination(Mat &ipm_frame, vector<Vec4f> &l)
{
    vector<Vec7i> tmp_lines;
    for(unsigned int it = 0; it < l.size(); ++it)
    {
        float x1 = l[it][0], y1 = l[it][1], x2 = l[it][2], y2 = l[it][3];
        if(y1 < y2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        float theta = fastAtan2((y2 - y1), (x2 - x1));
        if (theta > 180)
            theta -= 180;
        float length = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
        if ((threshold_angle_min < theta) && (theta < threshold_angle_max) && (length >= threshold_length))
            tmp_lines.push_back(Vec7i{(int)x1, (int)y1, (int)x2, (int)y2, (int)theta, (int)length, 0});
    }
    vector<vector<Point>> contours;
    vector<int> boundary_min = {0, 100, 0};
    vector<int> boundary_max = {154, 225, 154};
    cvtColor(ipm_frame, hsv_frame, COLOR_BGR2HSV);
    inRange(hsv_frame, boundary_min, boundary_max, hsv_frame);
    findContours(hsv_frame, contours, 1, 2);
    lines.clear();
    if(contours.empty())
    {
        lines = tmp_lines;
        return hsv_frame.clone();
    }
    for(unsigned int it_lines = 0; it_lines < tmp_lines.size(); ++it_lines)
    {
        int flag = 0;
        for(unsigned int it_contours = 0; it_contours < contours.size(); ++it_contours)
        {
            if (contourArea(contours[it_contours]) > 10000 || contourArea(contours[it_contours]) < 1000)
                continue;
//            qDebug() << contourArea(contours[it_contours]);
//            drawContours(ipm_frame, contours, it_contours, Scalar(0, 0, 255), 2);
            Point2f a((float)tmp_lines[it_lines][0], (float)tmp_lines[it_lines][1]);
            Point2f b((float)tmp_lines[it_lines][2], (float)tmp_lines[it_lines][3]);
            double dist1 = pointPolygonTest(contours[it_contours], a, true);
            double dist2 = pointPolygonTest(contours[it_contours], b, true);
            if(dist1 >= -17 || dist2 >= -17)
            {
                flag = 1;
                break;
            }
        }
        if(!flag)
            lines.push_back(Vec7i{tmp_lines[it_lines][0], tmp_lines[it_lines][1], tmp_lines[it_lines][2],
                                  tmp_lines[it_lines][3], tmp_lines[it_lines][4], tmp_lines[it_lines][5],
                                  tmp_lines[it_lines][6]});
    }
    return hsv_frame.clone();
}
