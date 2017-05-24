#include "curvefit.h"

CurveFit::CurveFit()
{

}

bool CurveFit::fromLinesToPoints(vector<Vec7i> &lines, int side)
{
    vector<Vec7i> *l;
    vector<Vec2i> *before;
    real_1d_array *beforeX;
    real_1d_array *beforeY;
    if (side == CurveFitting::left_points)
    {
        before = &leftPtsBeforeFit;
        beforeX = &leftXBeforeFit;
        beforeY = &leftYBeforeFit;
    }
    else if (side == CurveFitting::right_points)
    {
        before = &rightPtsBeforeFit;
        beforeX = &rightXBeforeFit;
        beforeY = &rightYBeforeFit;
    }
    if (lines.size() <= 1)
        return false;
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
    return true;
}

void CurveFit::setParameters(int start, int end, int n)
{
    startY = start; endY = end; ptsNum = n;
}

bool CurveFit::doCurveFitting(int side)
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
        case CurveFitting::left_points:
        ptsAfterFit = &leftPtsAfterFit;
        ptsX = leftXBeforeFit;
        ptsY = leftYBeforeFit;
        break;
        case CurveFitting::right_points:
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
    if (ptsAfterFit->at(0)[0] < -1000 || ptsAfterFit->at(0)[0] > 1000||  ptsAfterFit->at(ptsNum-1)[0] < -1000 || ptsAfterFit->at(ptsNum-1)[0] > 1000)
        return false;
    return true;
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
        case CurveFitting::left_points:
        return &leftPtsBeforeFit;
        case CurveFitting::right_points:
        return &rightPtsBeforeFit;
    }
}

vector<Vec2f> *CurveFit::getPtsAfterFit(int side)
{
    switch(side)
    {
        case CurveFitting::left_points:
        return &leftPtsAfterFit;
        case CurveFitting::right_points:
        return &rightPtsAfterFit;
    }
}
