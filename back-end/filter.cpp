#include "filter.h"

Filter::Filter()
{

}

vector<Vec7i>* Filter::getFilteredLines()
{
    return &lines;
}

void Filter::falseDetectionElimination(Mat &ipm_frame, vector<Vec4i> &l)
{
    vector<Vec7i> tmp_lines;
    for(unsigned int it = 0; it < l.size(); ++it)
    {
        int x1 = l[it][0], y1 = l[it][1], x2 = l[it][2], y2 = l[it][3];
        if(y1 > y2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        float theta = fastAtan2((y2 - y1), (x2 - x1));
        if (theta > 180)
            theta -= 180;
        float length = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
        if ((threshold_angle_min < theta) && (theta < threshold_angle_max) && (length >= threshold_length))
            tmp_lines.push_back(Vec7i{x1, y1, x2, y2, (int)theta, (int)length, 0});
    }
    vector<vector<Point>> contours;
    vector<int> boundary_min = {0, 100, 0};
    vector<int> boundary_max = {154, 255, 154};
    Mat hsv_frame;
    cvtColor(ipm_frame, hsv_frame, COLOR_BGR2HSV);
    inRange(hsv_frame, boundary_min, boundary_max, hsv_frame);
    findContours(hsv_frame, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    lines.clear();
    for(unsigned int it_lines = 0; it_lines < tmp_lines.size(); ++it_lines)
    {
        int flag = 0;
        if(contours.empty())
        {
            lines = tmp_lines;
            return;
        }
        for(unsigned int it_contours = 0; it_contours < contours.size(); ++it_contours)
        {
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
}
