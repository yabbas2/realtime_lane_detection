#include "region_growing.h"


RegionGrowing::RegionGrowing(){}

void RegionGrowing::regionGrowing(vector<Vec4i> lines, int &width)
{
    sort(lines.begin(), lines.end(),
                  [](const std::vector<float>& a, const std::vector<float>& b) {
          return a[1] > b[1];
        });

    for (int i = 0; i < lines.size(); ++i)
        if (lines[i] < width/2)
            leftLines.push_back(lines[i]);
        else
            rightLines.push_back(lines[i]);
    findLeftSeedLines();
    findRightSeedLines();
    leftRegionGrowing();
    rightRegionGrowing();
}

void RegionGrowing::findSeedLines(char &c)
{
    vector<Vec4i>* lines;
    if(c == "l")
        lines = &leftLines;
    else
        lines = &rightLines;
    int flag= 0;
    for (int i = 0; i < lines->size(); ++i) {
        if(flag)
            return;
        for (int j = 0; j < lines->size(); ++j) {
            if (lines->at(i) == lines->at(j))
                continue;
            if(abs(lines->at(i)[0]-lines->at(j)[0]) <= 20 && abs(lines->at(i)[1]-lines->at(j)[1]) <= 5)
            {
                if(c == "l")
                    leftSeedLine = lines->at(i);
                else
                    rightSeedLine = lines->at(i);
                flag = 1;
                break;
            }
        }
    }
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

void RegionGrowing::leftRegionGrowing()
{

}

void RegionGrowing::rightRegionGrowing()
{

}

vector<Vec4i> RegionGrowing::getLines()
{

}

Vec4i RegionGrowing::getLeftSeedLines()
{

}

Vec4i RegionGrowing::getRightSeedLines()
{

}

vector<Vec4i> RegionGrowing::getLeftRegion()
{

}

vector<Vec4i> RegionGrowing::getRightRegion()
{

}

