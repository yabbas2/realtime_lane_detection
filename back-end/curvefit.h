#ifndef CURVEFIT_H
#define CURVEFIT_H

#include <libalglib/stdafx.h>
#include <libalglib/interpolation.h>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace alglib;
using namespace std;
using namespace cv;

typedef Vec<int,7> Vec7i;

namespace CurveFitting {
    enum {left_points, right_points};
}

class CurveFit
{
public:
    explicit CurveFit();
    bool fromLinesToPoints(vector<Vec7i> &leftLines, int side);
    bool doCurveFitting(int side);
    void setParameters(int start, int end, int n);
    vector<Vec2i> *getPtsBeforeFit(int side);
    vector<Vec2f> *getPtsAfterFit(int side);

private:
    vector<Vec2i> leftPtsBeforeFit;
    vector<Vec2i> rightPtsBeforeFit;
    real_1d_array leftXBeforeFit;
    real_1d_array leftYBeforeFit;
    real_1d_array rightXBeforeFit;
    real_1d_array rightYBeforeFit;
    vector<float> newPtsY;
    vector<Vec2f> leftPtsAfterFit;
    vector<Vec2f> rightPtsAfterFit;
    int ptsNum, startY, endY;

    void makeLinspace();
};

#endif // CURVEFIT_H
