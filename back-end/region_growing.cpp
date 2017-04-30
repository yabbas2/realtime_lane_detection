#include "region_growing.h"


RegionGrowing::RegionGrowing(){}

void RegionGrowing::regionGrowing(vector<Vec4i> lines, int &width)
{
    sort(lines.begin(), lines.end(),
                  [](const Vec4i& a, const Vec4i& b) {
          return a[1] > b[1];
        });

    for (unsigned int i = 0; i < lines.size(); ++i)
        if (lines[i][0] < width/2)
            leftLines.push_back(lines[i]);
        else
            rightLines.push_back(lines[i]);
    findLeftSeedLines();
    findRightSeedLines();
    leftRegionGrowing();
    rightRegionGrowing();
}

void RegionGrowing::findLeftSeedLines()
{
    findSeedLines('l');
    if(leftSeedLine[0] == 0 && leftSeedLine[1] == 0 && leftSeedLine[2] == 0 && leftSeedLine[3] == 0)
        leftSeedLine = leftLines[0];
}

void RegionGrowing::findRightSeedLines()
{
    findSeedLines('r');
    if(rightSeedLine[0] == 0 && rightSeedLine[1] == 0 && rightSeedLine[2] == 0 && rightSeedLine[3] == 0)
        rightSeedLine = rightLines[0];
}

void RegionGrowing::findSeedLines(char c)
{
    vector<Vec4i>* lines;
    if(c == 'l')
        lines = &leftLines;
    else
        lines = &rightLines;
    int flag= 0;
    for (unsigned int i = 0; i < lines->size(); ++i) {
        if(flag)
            return;
        for (unsigned int j = 0; j < lines->size(); ++j) {
            if (lines->at(i) == lines->at(j))
                continue;
            if(abs(lines->at(i)[0]-lines->at(j)[0]) <= 20 && abs(lines->at(i)[1]-lines->at(j)[1]) <= 5)
            {
                if(c == 'l')
                    leftSeedLine = lines->at(i);
                else
                    rightSeedLine = lines->at(i);
                flag = 1;
                break;
            }
        }
    }
}

void RegionGrowing::leftRegionGrowing()
{
    if(leftSeedLine[0] == 0 && leftSeedLine[1] == 0 && leftSeedLine[2] == 0 && leftSeedLine[3] == 0)
        return;
    anyRegionGrowing('l');
}

void RegionGrowing::rightRegionGrowing()
{
    if(rightSeedLine[0] == 0 && rightSeedLine[1] == 0 && rightSeedLine[2] == 0 && rightSeedLine[3] == 0)
        return;
    anyRegionGrowing('r');
}

void RegionGrowing::anyRegionGrowing(char c)
{
    Vec4i* seedLine;
    vector<Vec4i>* region;
    vector<Vec4i>* lines;
    if (c == 'l')
    {
        seedLine = &leftSeedLine;
        region = &leftRegion;
        lines = &leftLines;
    }
    else
    {
        seedLine = &rightSeedLine;
        region = &rightRegion;
        lines = &rightLines;
    }
    (*seedLine)[6] = USED;
    region->push_back(*seedLine);
    int sumAngle, regionX, theta;
    double regionAngle;
    sumAngle = (*seedLine)[4];
    regionX = (*seedLine)[2];
    regionAngle = sumAngle / region->size();
    for (unsigned int i = 0; i < lines->size(); ++i) {
        theta = (*lines)[i][4];
        if((*lines)[i][6] == USED)
            continue;
        if (abs(regionAngle - theta) <= thresholdAngle) {
            if (abs(regionX - (*lines)[i][0]) <= thresholdX) {
                (*lines)[i][6] = USED;
                (*region).push_back((*lines)[i]);
                regionX = (*lines)[i][2];
                sumAngle += theta;
                regionAngle = sumAngle / (*region).size();
            }
            else if(abs(regionX - (*lines)[i][2]) <= 20) //note: we can check for delta y too
            {
                (*lines)[i][6] = USED;
                (*region).push_back((*lines)[i]);
                sumAngle += theta;
                regionX = (*lines)[i][2];
                regionAngle = sumAngle / (*region).size();
            }
        }
    }
}


vector<Vec4i> RegionGrowing::getLeftLines()
{
    return leftLines;
}

vector<Vec4i> RegionGrowing::getRightLines()
{
    return rightLines;
}

Vec4i RegionGrowing::getLeftSeedLines()
{
    return leftSeedLine;
}

Vec4i RegionGrowing::getRightSeedLines()
{
    return rightSeedLine;
}

vector<Vec4i> RegionGrowing::getLeftRegion()
{
    return leftRegion;
}

vector<Vec4i> RegionGrowing::getRightRegion()
{
    return rightRegion;
}

