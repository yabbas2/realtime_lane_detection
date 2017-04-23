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

namespace CurveFitting {
    enum {left_points, right_points};
}

class CurveFit
{
public:
    explicit CurveFit();
    void fromLinesToPoints(vector<Vec4i> &leftLines, vector<Vec4i> &rightLines);
    void doCurveFitting(int side);
    void setParameters(int start, int end, int n);
    vector<Vec2i> *getLeftPtsBeforeFit();
    vector<Vec2i> *getRightPtsBeforeFit();
    vector<Vec2i> *getLeftPtsAfterFit();
    vector<Vec2i> *getRightPtsAfterFit();

private:
    vector<Vec2i> leftPtsBeforeFit;
    vector<Vec2i> rightPtsBeforeFit;
    real_1d_array leftXBeforeFit;
    real_1d_array leftYBeforeFit;
    real_1d_array rightXBeforeFit;
    real_1d_array rightYBeforeFit;
    vector<double> newPtsY;
    vector<Vec2i> leftPtsAfterFit;
    vector<Vec2i> rightPtsAfterFit;
    int ptsNum, startY, endY;

    void makeLinspace();
};

#endif // CURVEFIT_H
