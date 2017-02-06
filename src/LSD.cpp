#include "Processing.hpp"

using namespace cv;

vector<Vec4f>Processing::lines;
list<Vec4f>Processing::detectedLanes;
int Processing::lineMargin = 0;
int Processing::marginCount = 0;
string Processing::lanesNumber = "zero";

void Processing::LSD()
{
    int width = ipmFrame.size().width;
    int leftMargin = 0, rightMargin = 0;
    cvtColor(ipmFrame, ipmFrame, COLOR_BGR2GRAY);
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector(LSD_REFINE_STD);
    lsd->detect(ipmFrame, lines);
    marginCount++;
    if(marginCount == (int)fps * 2)
    {
        marginCount = 0;
        leftMargin = checkLanesFromLeft();
        rightMargin = checkLanesFromRight();
        if(leftMargin == rightMargin && leftMargin > 0)
            lineMargin = leftMargin;
        else
        {
            if(rightMargin == 0 && leftMargin > 0)
                lineMargin = leftMargin;
            else if(leftMargin == 0 && rightMargin > 0)
                lineMargin = rightMargin;
        }
        switch(lineMargin)
        {
            case 280: lanesNumber = "one"; break;
            case 120: lanesNumber = "two"; break;
            case 66: lanesNumber = "three"; break;
            case 40: lanesNumber = "four"; break;
            default: break;
        }
    }
    filterAndTakeAverage(0, width, 40, lineMargin);
}

void Processing::filterAndTakeAverage(int start, int end, unsigned int windowSize, int margin)
{
    int height = ipmFrame.size().height;
    float sum_x1, sum_x2, avg_x1, avg_x2;
    unsigned int size;
    int i;
    vector<Vec4f>::iterator j;
    detectedLanes.clear();

    /*#pragma omp parallel for default(none) private(detectedList, sum_x1, sum_x2, avg_x1, avg_x2,\
        i, j, line, size) shared(height, threshold, windowSize, start, end, filteredLines)*/

    for(i = start; i <= end; i += windowSize + margin)
    {
        sum_x1 = 0.0; sum_x2 = 0.0; size = 0;
        for(j = lines.begin(); j != lines.end(); j++)
        {
            if((*j)[0] >= i && (*j)[0] < i+windowSize && (*j)[2] >= i && (*j)[2] < i+windowSize)
            {
                sum_x1 += (*j)[0]; sum_x2 += (*j)[2];
                size++;
            }
        }
        avg_x1 = sum_x1/size; avg_x2 = sum_x2/size;
        //#pragma omp critical
        {
            detectedLanes.push_back(Vec4d{avg_x1, 0, avg_x2, (float) height});
        }
    }

}

int Processing::checkLanesFromLeft()
{
    int margin = 0;
    sort(begin(lines), end(lines), [](const Vec4f &a, const Vec4f &b) { return a[0] < b[0]; });
    vector<Vec4f>::iterator j = lines.begin();
    while ((int)(*j)[0] >= 0 && (int)(*j)[0] <= 50) j++;

    if ((int)(*j)[0] >= 310 && (int)(*j)[0] <= 360) //Two lanes
        margin = 280;
    else if ((int)(*j)[0] >= 150 && (int)(*j)[0] <= 210) //Three lanes
        margin = 120;

    if ((int)(*j)[0] >= 96 && (int)(*j)[0] <= 156) //Four lanes
    {
        while ((int)(*j)[0] >= 96 && (int)(*j)[0] <= 156) j++;
        if((int)(*j)[0] >= 202 && (int)(*j)[0] <= 262)
            margin = 66;
    }

    if ((int)(*j)[0] >= 70 && (int)(*j)[0] <= 130) //Five lanes
    {
        while ((int)(*j)[0] >= 70 && (int)(*j)[0] <= 130) j++;
        if((int)(*j)[0] >= 150 && (int)(*j)[0] <= 210)
            margin = 40;
    }
    return margin;
}

int Processing::checkLanesFromRight()
{
    int margin = 0;
    sort(begin(lines), end(lines), [](const Vec4f &a, const Vec4f &b) { return a[0] > b[0]; });
    vector<Vec4f>::iterator j = lines.begin();
    while ((int)(*j)[0] <= 360 && (int)(*j)[0] >= 310) j++;

    if ((int)(*j)[0] <= 50 && (int)(*j)[0] >= 0) //Two lanes
        margin = 280;
    else if ((int)(*j)[0] <= 210 && (int)(*j)[0] >= 150) //Three lanes
        margin = 120;

    if ((int)(*j)[0] <= 264 && (int)(*j)[0] >= 204) //Four lanes
    {
        while ((int)(*j)[0] <= 264 && (int)(*j)[0] >= 204) j++;
        if((int)(*j)[0] <= 158 && (int)(*j)[0] >= 98)
            margin = 66;
    }

    if ((int)(*j)[0] <= 290 && (int)(*j)[0] >= 230) //Five lanes
    {
        while ((int)(*j)[0] <= 290 && (int)(*j)[0] >= 230) j++;
        if((int)(*j)[0] <= 210 && (int)(*j)[0] >= 150)
            margin = 40;
    }
    return margin;
}