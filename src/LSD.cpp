#include "Processing.hpp"

using namespace cv;

vector<Vec4f>Processing::lines;
list<Vec4f>Processing::detectedLanes;
int Processing::margin = 0;
int Processing::marginCount = 0;
string Processing::lanesNumber = "0";

void Processing::LSD()
{
    int width = ipmFrame.size().width;
    cvtColor(ipmFrame, ipmFrame, COLOR_BGR2GRAY);
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector(LSD_REFINE_STD);
    lines.clear();
    lsd->detect(ipmFrame, lines);
    marginCount++;
    if(marginCount == (int)fps * 2) checkLanes();
    filterAndTakeAverage(0, width, 40, margin);
}

void Processing::filterAndTakeAverage(int start, int end, unsigned int windowSize, int margin)
{
    list<Vec4f> detectedList;
    int height = ipmFrame.size().height;
    float sum_x1, sum_x2, avg_x1, avg_x2;
    unsigned int size;
    int i;
    vector<Vec4f>::iterator j;
    list<Vec4f>::iterator line;
    detectedLanes.clear();

    /*#pragma omp parallel for default(none) private(detectedList, sum_x1, sum_x2, avg_x1, avg_x2,\
        i, j, line, size) shared(height, threshold, windowSize, start, end, filteredLines)*/

    for(i = start; i <= end; i += windowSize + margin)
    {
        sum_x1 = 0.0; sum_x2 = 0.0;
        detectedList.clear();
        for(j = lines.begin(); j != lines.end(); j++)
        {
            if((*j)[0] >= i && (*j)[0] < i+windowSize && (*j)[2] >= i && (*j)[2] < i+windowSize)
                detectedList.push_back(*j);
        }
        size = (unsigned int)detectedList.size();
        for(line = detectedList.begin(); line != detectedList.end(); line++)
        {
            sum_x1 += (*line)[0]; sum_x2 += (*line)[2];
        }
        avg_x1 = sum_x1/size; avg_x2 = sum_x2/size;
        //#pragma omp critical
        {
            detectedLanes.push_back(Vec4d{avg_x1, 0, avg_x2, (float) height});
        }
    }

}

void Processing::checkLanes()
{
    marginCount = 0;
    sort(begin(lines), end(lines), [](const Vec4f &a, const Vec4f &b) { return a[0] < b[0]; });
    vector<Vec4f>::iterator j = lines.begin();
    while ((int)(*j)[0] >= 0 && (int)(*j)[0] <= 50) j++;

    if ((int)(*j)[0] >= 310 && (int)(*j)[0] <= 370) //Two lanes
    {margin = 280; lanesNumber = "2";}
    else if ((int)(*j)[0] >= 150 && (int)(*j)[0] <= 210) //Three lanes
    {margin = 120; lanesNumber = "3";}

    if ((int)(*j)[0] >= 96 && (int)(*j)[0] <= 156) //Four lanes
    {
        while ((int)(*j)[0] >= 96 && (int)(*j)[0] <= 156) j++;
        if((int)(*j)[0] >= 202 && (int)(*j)[0] <= 262)
        {margin = 66; lanesNumber = "4";}
    }

    if ((int)(*j)[0] >= 70 && (int)(*j)[0] <= 130) //Five lanes
    {
        while ((int)(*j)[0] >= 70 && (int)(*j)[0] <= 130) j++;
        if((int)(*j)[0] >= 150 && (int)(*j)[0] <= 210)
        {margin = 40; lanesNumber = "5";}
    }
}