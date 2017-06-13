#ifndef CURVEFIT_H
#define CURVEFIT_H

#include <libalglib/stdafx.h>
#include <libalglib/interpolation.h>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

#define xThreshold      180
#define xRange          800

using namespace alglib;
using namespace std;
using namespace cv;

typedef Vec<int,7> Vec7i;

namespace CurveFitting {
    enum {left, right};
}

class CurveFit
{
public:
    explicit CurveFit();
    void doCurveFitting(int side);
    void setParameters(int start, int end, int n);
    vector<Vec2i> *getPtsBeforeFit(int side);
    vector<Vec2f> *getPtsAfterFit(int side);
    void validateLineBefore(vector<Vec7i> &lines, int side);
    void validateLineAfter();

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
    bool leftState, rightState;

    void makeLinspace();
    void fromLinesToPoints(vector<Vec7i> &leftLines, int side);
};

#endif // CURVEFIT_H
