#include "curvefit.h"

CurveFit::CurveFit()
{

}

void CurveFit::fromLinesToPoints(vector<Vec7i> &lines, int side)
{
    vector<Vec2i> *before;
    real_1d_array *beforeX;
    real_1d_array *beforeY;
    if (side == CurveFitting::left)
    {
        before = &leftPtsBeforeFit;
        beforeX = &leftXBeforeFit;
        beforeY = &leftYBeforeFit;
    }
    else if (side == CurveFitting::right)
    {
        before = &rightPtsBeforeFit;
        beforeX = &rightXBeforeFit;
        beforeY = &rightYBeforeFit;
    }
    vector<Vec7i>::iterator it;
    int i;
    before->clear();
    beforeX->setlength(2 * lines.size());
    beforeY->setlength(2 * lines.size());
    for(it = lines.begin(), i = 0; it != lines.end(); ++it, ++i)
    {
        before->push_back(Vec2i{(*it)[0], (*it)[1]});
        before->push_back(Vec2i{(*it)[2], (*it)[3]});
        beforeX->operator [](i) = (*it)[0];
        beforeY->operator [](i) = (*it)[1];
        beforeX->operator [](++i) = (*it)[2];
        beforeY->operator [](i) = (*it)[3];
    }
}

void CurveFit::setParameters(int start, int end, int n)
{
    startY = start; endY = end; ptsNum = n;
}

void CurveFit::doCurveFitting(int side)
{
    vector<Vec2f> *ptsAfterFit;
    real_1d_array ptsX;
    real_1d_array ptsY;
    ae_int_t m;
    ae_int_t info;
    barycentricinterpolant p;
    polynomialfitreport rep;
    switch(side)
    {
        case CurveFitting::left:
        if (!leftState)
            return;
        ptsAfterFit = &leftPtsAfterFit;
        ptsX = leftXBeforeFit;
        ptsY = leftYBeforeFit;
        break;
        case CurveFitting::right:
        if (!rightState)
            return;
        ptsAfterFit = &rightPtsAfterFit;
        ptsX = rightXBeforeFit;
        ptsY = rightYBeforeFit;
        break;
    }
    if (ptsX.length() <= 6)
        m = 2;
    else
        m = 3;
    polynomialfit(ptsY, ptsX, m, info, p, rep);
    makeLinspace();
    ptsAfterFit->clear();
    for (unsigned int i = 0; i < newPtsY.size(); ++i)
        ptsAfterFit->push_back(Vec2f{(float)barycentriccalc(p, newPtsY[i]), (float)newPtsY[i]});
}

void CurveFit::makeLinspace()
{
    newPtsY.clear();
    double a = startY, b = endY;
    double step = (b-a) / (ptsNum-1);
    while((int)a <= (int)b)
    {
        newPtsY.push_back(a);
        a += step;
    }
}

vector<Vec2i> *CurveFit::getPtsBeforeFit(int side)
{
    switch(side)
    {
        case CurveFitting::left:
        return &leftPtsBeforeFit;
        case CurveFitting::right:
        return &rightPtsBeforeFit;
    }
}

vector<Vec2f> *CurveFit::getPtsAfterFit(int side)
{
    switch(side)
    {
        case CurveFitting::left:
        return &leftPtsAfterFit;
        case CurveFitting::right:
        return &rightPtsAfterFit;
    }
}

bool CurveFit::validateLineBefore(vector<Vec7i> &lines, int side)
{
    if (lines.size() > 1 && side == CurveFitting::left)
    {
        leftState = true;
        fromLinesToPoints(lines, side);
    }
    else if (lines.size() > 1 && side == CurveFitting::right)
    {
        rightState = true;
        fromLinesToPoints(lines, side);
    }
    else if (lines.size() <= 1 && side == CurveFitting::left)
        leftState = false;
    else if (lines.size() <= 1 && side == CurveFitting::right)
        rightState = false;
    if (side == CurveFitting::left)
        return leftState;
    else if (side == CurveFitting::right)
        return rightState;
}

void CurveFit::validateLineAfter()
{
    if (!leftState)
        leftPtsAfterFit.clear();
    if (!rightState)
        rightPtsAfterFit.clear();

    if (leftState && (leftPtsAfterFit.at(0)[0] < -xRange || leftPtsAfterFit.at(0)[0] > 480 ||
            leftPtsAfterFit.at(ptsNum-1)[0] < -xRange || leftPtsAfterFit.at(ptsNum-1)[0] > 480))
        leftPtsAfterFit.clear();

    if (rightState && (rightPtsAfterFit.at(0)[0] < 0 || rightPtsAfterFit.at(0)[0] > xRange+480 ||
            rightPtsAfterFit.at(ptsNum-1)[0] < 0 || rightPtsAfterFit.at(ptsNum-1)[0] > xRange+480))
        rightPtsAfterFit.clear();

    if (leftState && rightState && leftPtsAfterFit.size() > 0 && rightPtsAfterFit.size() > 0)
    {
        int deltaX1 = abs(leftPtsAfterFit.at(0)[0] - rightPtsAfterFit.at(0)[0]);
        int deltaX2 = abs(leftPtsAfterFit.at(ptsNum-1)[0] - rightPtsAfterFit.at(ptsNum-1)[0]);
        if (deltaX1 <= xThreshold || deltaX2 <= xThreshold)
        {
            leftPtsAfterFit.clear();
            rightPtsAfterFit.clear();
        }
    }
}

CurveFit::~CurveFit()
{

}
