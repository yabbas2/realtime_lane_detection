#include "reg.h"

Reg::Reg(int &argc, char **argv) :
    QApplication(argc, argv)
{
    log.openFile(qApp->applicationDirPath() + "/../logger/logFiles/log_reg.txt", QIODevice::WriteOnly);
    log.write("----------------------------------------------------------");
    log.write("------------------------NEW RUN---------------------------");
    log.write("----------------------------------------------------------");
    ifMaster = new QDBusInterface("com.stage.master", "/", "com.stage.master", bus, this);
    QDBusReply<QString> key = ifMaster->call("getDETECTIONREGKEY");
    sm.setKey(key.value());
    sm.attach(QSharedMemory::ReadOnly);
    frameCount = 0;
    busy = false;
}

void Reg::process()
{
    busy = true;
    t1 = high_resolution_clock::now();
    frameCount++;
    leftLines.clear();
    rightLines.clear();
    while (!sm.lock());
    sharedData *sData = (sharedData*) sm.data();
    for (int i = 0; i < sData->actualSize; ++i)
    {
        int x1 = sData->lineSegments[i][0], y1 = sData->lineSegments[i][1], x2 = sData->lineSegments[i][2],
                y2 = sData->lineSegments[i][3], theta = sData->lineSegments[i][4], length = sData->lineSegments[i][5];
        if (x1 < frameWidth/2)
            leftLines.push_back(Vec7i{x1, y1, x2, y2, theta, length, NOT_USED});
        else
            rightLines.push_back(Vec7i{x1, y1, x2, y2, theta, length, NOT_USED});
    }
    sm.unlock();
    if (leftLines.size() > 0)
    {
        sort(leftLines.begin(), leftLines.end(), [](const Vec7i& a, const Vec7i& b) {return a[1] > b[1];});
        regionGrowing(REG::left);
        validateLineBefore(REG::left);
        curveFit(REG::left);
    }
    if (rightLines.size() > 0)
    {
        sort(rightLines.begin(), rightLines.end(), [](const Vec7i& a, const Vec7i& b) {return a[1] > b[1];});
        regionGrowing(REG::right);
        validateLineBefore(REG::right);
        curveFit(REG::right);
    }
    validateLineAfter();
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    log.write("[REG] frame no. " + QString::number(frameCount) + ", exec time: " + QString::number(duration));
    busy = false;
}

void Reg::regionGrowing(int option)
{
    if (option == REG::left)
    {
        leftRegion.clear();
        findLeftSeedLine();
        genRegGrow(REG::left);
    }
    else if (option == REG::right)
    {
        rightRegion.clear();
        findRightSeedLine();
        genRegGrow(REG::right);
    }
    else if (option == REG::both)
    {
        leftRegion.clear();
        rightRegion.clear();
        findLeftSeedLine();
        findRightSeedLine();
        genRegGrow(REG::left);
        genRegGrow(REG::right);
    }
}

void Reg::findLeftSeedLine()
{
    leftSeedLine = Vec7i{0, 0, 0, 0, 0, 0, 0};
    genSeedLine(REG::left);
    if(leftSeedLine[0] == 0 && leftSeedLine[1] == 0 && leftSeedLine[2] == 0 && leftSeedLine[3] == 0 && leftLines.size() > 0)
        leftSeedLine = leftLines[0];
}

void Reg::findRightSeedLine()
{
    rightSeedLine = Vec7i{0, 0, 0, 0, 0, 0, 0};
    genSeedLine(REG::right);
    if(rightSeedLine[0] == 0 && rightSeedLine[1] == 0 && rightSeedLine[2] == 0 && rightSeedLine[3] == 0 && rightLines.size() > 0)
        rightSeedLine = rightLines[0];
}

void Reg::genSeedLine(int c)
{
    vector<Vec7i>* lines;
    if (c == REG::left)
        lines = &leftLines;
    else if (c == REG::right)
        lines = &rightLines;
    bool foundFlag = false;
    for (unsigned int i = 0; i < lines->size(); ++i) {
        if(foundFlag)
            return;
        for (unsigned int j = 0; j < lines->size(); ++j) {
            if (lines->at(i) == lines->at(j))
                continue;
            if(abs(lines->at(i)[0]-lines->at(j)[0]) <= 20 && abs(lines->at(i)[1]-lines->at(j)[1]) <= 5)
            {
                if(c == REG::left)
                {
                    leftSeedLine = lines->at(i);
                    lines->at(i)[6] = USED;
                }
                else if (c == REG::right)
                {
                    rightSeedLine = lines->at(i);
                    lines->at(i)[6] = USED;
                }
                foundFlag = true;
                break;
            }
        }
    }
}

void Reg::genRegGrow(int c)
{
    Vec7i* seedLine;
    vector<Vec7i>* region;
    vector<Vec7i>* lines;
    if (c == REG::left)
    {
        seedLine = &leftSeedLine;
        region = &leftRegion;
        lines = &leftLines;
    }
    else if (c == REG::right)
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
            if (abs(regionX - (*lines)[i][0]) <= thresholdX || abs(regionX - (*lines)[i][2]) <= thresholdX)
            {
                (*region).push_back((*lines)[i]);
                (*lines)[i][6] = USED;
                regionX = (*lines)[i][2];
                sumAngle += theta;
                regionAngle = sumAngle / (*region).size();
            }
        }
    }
}

void Reg::fromLinesToPoints(int side)
{
    vector<Vec7i> *region;
    real_1d_array *beforeX;
    real_1d_array *beforeY;
    if (side == REG::left)
    {
        region = &leftRegion;
        beforeX = &leftXBeforeFit;
        beforeY = &leftYBeforeFit;
    }
    else if (side == REG::right)
    {
        region = &rightRegion;
        beforeX = &rightXBeforeFit;
        beforeY = &rightYBeforeFit;
    }
    vector<Vec7i>::iterator it;
    int i;
    beforeX->setlength(2 * region->size());
    beforeY->setlength(2 * region->size());
    for(it = region->begin(), i = 0; it != region->end(); ++it, ++i)
    {
        beforeX->operator [](i) = (*it)[0];
        beforeY->operator [](i) = (*it)[1];
        beforeX->operator [](++i) = (*it)[2];
        beforeY->operator [](i) = (*it)[3];
    }
}

void Reg::curveFit(int side)
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
        case REG::left:
        if (!leftState)
            return;
        ptsAfterFit = &leftPtsAfterFit;
        ptsX = leftXBeforeFit;
        ptsY = leftYBeforeFit;
        break;
        case REG::right:
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

void Reg::makeLinspace()
{
    newPtsY.clear();
    double a = START_Y, b = END_Y;
    double step = (b-a) / (PTS_NUM-1);
    while((int)a <= (int)b)
    {
        newPtsY.push_back(a);
        a += step;
    }
}

void Reg::validateLineBefore(int side)
{
    vector<Vec7i>* region;
    bool *state;
    if (side == REG::left)
    {
        region = &leftRegion;
        state = &leftState;
    }
    else if (side == REG::right)
    {
        region = &rightRegion;
        state = &rightState;
    }
    if (region->size() > 1)
    {
        *state = true;
        fromLinesToPoints(side);
    }
    else if (region->size() <= 1)
        *state = false;
}

void Reg::validateLineAfter()
{
    if (!leftState)
        leftPtsAfterFit.clear();
    if (!rightState)
        rightPtsAfterFit.clear();

    if (leftState && (leftPtsAfterFit.at(0)[0] < -outOfRangeThreshold || leftPtsAfterFit.at(0)[0] > 480 ||
            leftPtsAfterFit.at(PTS_NUM-1)[0] < -outOfRangeThreshold || leftPtsAfterFit.at(PTS_NUM-1)[0] > 480))
        leftPtsAfterFit.clear();

    if (rightState && (rightPtsAfterFit.at(0)[0] < 0 || rightPtsAfterFit.at(0)[0] > outOfRangeThreshold+480 ||
            rightPtsAfterFit.at(PTS_NUM-1)[0] < 0 || rightPtsAfterFit.at(PTS_NUM-1)[0] > outOfRangeThreshold+480))
        rightPtsAfterFit.clear();

    if (leftState && rightState && leftPtsAfterFit.size() > 0 && rightPtsAfterFit.size() > 0)
    {
        int deltaX1 = abs(leftPtsAfterFit.at(0)[0] - rightPtsAfterFit.at(0)[0]);
        int deltaX2 = abs(leftPtsAfterFit.at(PTS_NUM-1)[0] - rightPtsAfterFit.at(PTS_NUM-1)[0]);
        if (deltaX1 <= overlapThreshold || deltaX2 <= overlapThreshold)
        {
            leftPtsAfterFit.clear();
            rightPtsAfterFit.clear();
        }
    }
}
