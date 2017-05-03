#include "curvefit.h"

CurveFit::CurveFit()
{

}

bool CurveFit::fromLinesToPoints(vector<Vec7i> &leftLines, vector<Vec7i> &rightLines)
{
    if (leftLines.empty() || rightLines.empty())
        return false;
    vector<Vec7i>::iterator it;
    int i;
    leftPtsBeforeFit.clear();
    rightPtsBeforeFit.clear();
    leftXBeforeFit.setlength(2 * leftLines.size());
    leftYBeforeFit.setlength(2 * leftLines.size());
    rightXBeforeFit.setlength(2 * rightLines.size());
    rightYBeforeFit.setlength(2 * rightLines.size());
    for(it = leftLines.begin(), i = 0; it != leftLines.end(); ++it, ++i)
    {
        leftPtsBeforeFit.push_back(Vec2i{(*it)[0], (*it)[1]});
        leftPtsBeforeFit.push_back(Vec2i{(*it)[2], (*it)[3]});
        leftXBeforeFit[i] = (*it)[0];
        leftYBeforeFit[i] = (*it)[1];
        leftXBeforeFit[++i] = (*it)[2];
        leftYBeforeFit[i] = (*it)[3];
    }
    for(it = rightLines.begin(), i = 0; it != rightLines.end(); ++it, ++i)
    {
        rightPtsBeforeFit.push_back(Vec2i{(*it)[0], (*it)[1]});
        rightPtsBeforeFit.push_back(Vec2i{(*it)[2], (*it)[3]});
        rightXBeforeFit[i] = (*it)[0];
        rightYBeforeFit[i] = (*it)[1];
        rightXBeforeFit[++i] = (*it)[2];
        rightYBeforeFit[i] = (*it)[3];
    }
    return true;
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
    if (ptsX.length() <= 4)
        m = 2;
    else
        m = 3;
    polynomialfit(ptsY, ptsX, m, info, p, rep);
    makeLinspace();
    ptsAfterFit->clear();
    for (unsigned int i = 0; i < newPtsY.size(); ++i)
        ptsAfterFit->push_back(Vec2f{barycentriccalc(p, newPtsY[i]), newPtsY[i]});
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

vector<Vec2i> *CurveFit::getLeftPtsBeforeFit()
{
    return &leftPtsBeforeFit;
}

vector<Vec2i> *CurveFit::getRightPtsBeforeFit()
{
    return &rightPtsBeforeFit;
}

vector<Vec2f> *CurveFit::getLeftPtsAfterFit()
{
    return &leftPtsAfterFit;
}

vector<Vec2f> *CurveFit::getRightPtsAfterFit()
{
    return &rightPtsAfterFit;
}
