#include "Processing.hpp"

using namespace cv;

vector<Vec4f>Processing::lines;
list<Vec4f>Processing::detectedLanes;
int Processing::marginCount = 0;
int Processing::margin = 0;

void Processing::LSD()
{
    int width = ipmFrame.size().width;
    cvtColor(ipmFrame, ipmFrame, COLOR_BGR2GRAY);
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector(LSD_REFINE_STD);
    lines.clear();
    lsd->detect(ipmFrame, lines);
    margin += checkLanes(40, 1);
//    if(marginCount < 2)
//    {
//        margin += checkLanes(40, 1);
//        margin = margin / marginCount;
//        cout << margin << endl;
//    }
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

int Processing::checkLanes(int windowSize, int threshold)
{
    sort(begin(lines), end(lines), [](const Vec4f &a, const Vec4f &b) {return a[0] < b[0];});
    int size = lines.size();
    marginCount++;
    int margin;
    vector<Vec4f>::iterator j;
    int D = 0;
    for(j = lines.begin(); j != lines.end(); j++)
    {
        if((*j)[0] >= 160 && (*j)[0] < 160+windowSize && (*j)[2] >= 160 && (*j)[2] < 160+windowSize)
            D++;
    }
    if(D >= threshold)
    {
        D = 0;
        for(j = lines.begin(); j != lines.end(); j++)
        {
            if(((*j)[0] >= 80 && (*j)[0] < 80+windowSize && (*j)[2] >= 80 && (*j)[2] < 80+windowSize)
               || ((*j)[0] >= 240 && (*j)[0] < 240+windowSize && (*j)[2] >= 240 && (*j)[2] < 240+windowSize))
                D++;
        }
        if(D >= 2*threshold)
            margin = 40;
        else
            margin = 120;
        return margin;
    }
    D = 0;
    for(j = lines.begin(); j != lines.end(); j++)
    {
        if(((*j)[0] >= 106 && (*j)[0] < 106+windowSize && (*j)[2] >= 106 && (*j)[2] < 106+windowSize)
           || ((*j)[0] >= 212 && (*j)[0] < 212+windowSize && (*j)[2] >= 212 && (*j)[2] < 212+windowSize))
            D++;
    }
    if(D >= 2*threshold)
        margin = 66;
    else
        margin = 280;
    return margin;
}