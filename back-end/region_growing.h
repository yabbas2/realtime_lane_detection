#ifndef REGION_GROWING_H
#define REGION_GROWING_H

#include<opencv2/opencv.hpp>
#include <vector>

#define USED                1
#define threshold_angle     10
#define threshold_x         40

using namespace std;
using namespace cv;

class RegionGrowing
{
private:
    vector<Vec4i> leftLines;
    vector<Vec4i> rightLines;
    Vec4i leftSeedLine;
    Vec4i rightSeedLine;
    vector<Vec4i> leftRegion;
    vector<Vec4i> rightRegion;
public:
    RegionGrowing();

    void regionGrowing(vector<Vec4i> lines, int &width);
    void findSeedLines(char &c);
    void findLeftSeedLines();
    void findRightSeedLines();
    void leftRegionGrowing();
    void rightRegionGrowing();

    vector<Vec4i> getLines();
    Vec4i getLeftSeedLines();
    Vec4i getRightSeedLines();
    vector<Vec4i> getLeftRegion();
    vector<Vec4i> getRightRegion();

};

#endif // REGION_GROWING_H
