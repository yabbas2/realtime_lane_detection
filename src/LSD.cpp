#include <opencv2/opencv.hpp>
#include "Processing.hpp"

using namespace cv;

void Processing::LSD()
{
    int width = ipmFrame.size().width;
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector(LSD_REFINE_STD);
    lsd->detect(ipmFrame, lines);
    filterAndTakeAverage(0, width, 40, 0);
}

void Processing::filterAndTakeAverage(int start, int end, unsigned int windowSize, unsigned int threshold)
{
    list<Vec4d> filteredLines;
    list<Vec4d> detectedList;
    double slope;
    int height = ipmFrame.size().height;
    double sum_x1, sum_x2, avg_x1, avg_x2;
    unsigned int size;
    int i;
    size_t k;
    list<Vec4d>::iterator j;
    list<Vec4d>::iterator line;
    detectedLanes.clear();
    for(k = 0; k < lines.size(); k++)
    {
        slope = atan2(lines[k][3] - lines[k][1], lines[k][2] - lines[k][0]);
        if (abs(slope) <= 95 * M_PI / 180 && abs(slope) >= 85 * M_PI / 180)
        {
            filteredLines.push_back(lines[k]);
        }
    }
    #pragma omp parallel for default(none) private(detectedList, sum_x1, sum_x2, avg_x1, avg_x2, slope,\
        i, j, line, size) shared(height, threshold, windowSize, start, end)
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
        #pragma omp critical
        {
            detectedLanes.push_back(Vec4d{avg_x1, 0, avg_x2, (double) height});
        }
    }
}