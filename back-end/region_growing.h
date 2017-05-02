#ifndef REGION_GROWING_H
#define REGION_GROWING_H

#define USED                1
#define thresholdAngle     10
#define thresholdX         40


#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

typedef Vec<int, 7> Vec7i;

class RegionGrowing
{
private:
    vector<Vec7i> leftLines;
    vector<Vec7i> rightLines;
    Vec7i leftSeedLine;
    Vec7i rightSeedLine;
    vector<Vec7i> leftRegion;
    vector<Vec7i> rightRegion;
    void findSeedLines(char c);
    void findLeftSeedLines();
    void findRightSeedLines();
    void anyRegionGrowing(char c);
    void leftRegionGrowing();
    void rightRegionGrowing();
public:
    RegionGrowing();
    void regionGrowing(vector<Vec7i> lines, int &width);
    vector<Vec7i> *getLeftLines();
    vector<Vec7i> *getRightLines();
    Vec7i *getLeftSeedLines();
    Vec7i *getRightSeedLines();
    vector<Vec7i> *getLeftRegion();
    vector<Vec7i> *getRightRegion();

};

#endif // REGION_GROWING_H
