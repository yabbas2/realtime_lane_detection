#include "Processing.hpp"

using namespace cv;

vector<Vec4f>Processing::lines;
list<Vec4f>Processing::detectedLanes;

void Processing::LSD()
{
    int width = ipmFrame.size().width;
    cvtColor(ipmFrame, ipmFrame, COLOR_BGR2GRAY);
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector(LSD_REFINE_STD);
    lines.clear();
    lsd->detect(ipmFrame, lines);
    filterAndTakeAverage(0, width, 100, 0);
}

void Processing::filterAndTakeAverage(int start, int end, unsigned int windowSize, unsigned int threshold)
{
    list<Vec4f> filteredLines;
    list<Vec4f> detectedList;
    float slope;
    int height = ipmFrame.size().height;
    float sum_x1, sum_x2, avg_x1, avg_x2;
    unsigned int size;
    int i;
    vector<Vec4f>::iterator k;
    list<Vec4f>::iterator j;
    list<Vec4f>::iterator line;
    detectedLanes.clear();
    filteredLines.clear();
    for(k = lines.begin(); k != lines.end(); k++)
    {
        slope = fastAtan2((*k)[3] - (*k)[1], (*k)[2] - (*k)[0]);
        if (abs(slope) <= 95 && abs(slope) >= 85)
        {
            filteredLines.push_back(*k);
        }
    }
    /*#pragma omp parallel for default(none) private(detectedList, sum_x1, sum_x2, avg_x1, avg_x2,\
        i, j, line, size) shared(height, threshold, windowSize, start, end, filteredLines)*/
    for(i = start; i <= end; i += windowSize)
    {
        sum_x1 = 0.0; sum_x2 = 0.0;
        detectedList.clear();
        for(j = filteredLines.begin(); j != filteredLines.end(); j++)
        {
            if((*j)[0] >= i && (*j)[0] < i+windowSize)
                detectedList.push_back(*j);
        }
        size = (unsigned int)detectedList.size();
        if(size < threshold)
            continue;
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