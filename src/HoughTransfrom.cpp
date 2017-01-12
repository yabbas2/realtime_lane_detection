#include "Processing.hpp"

vector<Vec4d>Processing::lines = vector<Vec4d>(0, 0);
list<Vec4d>Processing::detectedLanes;

void Processing::houghTransform()
{
    int width = ipmFrame.size().width;
    unsigned char minLineLength = 60;
    unsigned char maxLineGap = 1;
    unsigned char maxVotes = 90;
    HoughLinesP(ipmFrame, lines, 1, M_PI/180, minLineLength, maxLineGap, maxVotes);
    takeAverage(0, width, 40, 0);
}
void Processing::takeAverage(int start, int end, unsigned int windowSize, unsigned int threshold)
{
    list<Vec4d> detectedList;
    unsigned int counter;
    int height = ipmFrame.size().height;
    double sum_x1, sum_x2, avg_x1, avg_x2;
    double slope;
    int i;
    size_t j;
    list<Vec4d>::iterator line;
    detectedLanes.clear();
    #pragma omp parallel for default(none) private(detectedList, counter, sum_x1, sum_x2, avg_x1, avg_x2, slope,\
    i, j, line) shared(height, threshold, windowSize, start, end)
    for(i = start; i <= end; i += windowSize)
    {
        counter = 0; sum_x1 = 0.0; sum_x2 = 0.0;
        detectedList.clear();
        for(j = 0; j < lines.size(); j++)
        {
            if(lines[j][0] >= i && lines[j][0] <= i+windowSize)
                detectedList.push_back(lines[j]);
        }
        if(!detectedList.empty())
        {
            for(line = detectedList.begin(); line != detectedList.end(); line++)
            {
                counter++;
                sum_x1 += (*line)[0]; sum_x2 += (*line)[2];
            }
        }
        if(counter > threshold)
        {
            avg_x1 = sum_x1/counter; avg_x2 = sum_x2/counter;
            slope = atan2(height - 0, avg_x2 - avg_x1);
            #pragma omp critical
            {
                if (abs(slope) <= 95 * M_PI / 180 && abs(slope) >= 85 * M_PI / 180)
                    detectedLanes.push_back(Vec4d{avg_x1, 0, avg_x2, (double) height});
            }
        }
    }
}