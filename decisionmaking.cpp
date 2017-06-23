#include "decisionmaking.h"


DecisionMaking::DecisionMaking()
{
    leftStatus[0] = 0;
    rightStatus[0] = 0;
    leftStatus[1] = 0;
    rightStatus[1] = 0;
}

void DecisionMaking::decideType(vector<Vec7i> &lines, Vec7i &seed_line, int side)
{
    int* status;
    if (side == Decision::left)
        status = &(leftStatus[0]);
    else if (side == Decision::right)
        status = &(rightStatus[0]);
    if(lines.empty())
    {
        if(*status > 0)
            *status = *status - 1;
        else if (*status < 0)
            *status = *status + 1;
        return;
    }
    int dashed = 0;
    int solid = 0;
    sort(lines.begin(), lines.end(),
              [](const Vec7i& a, const Vec7i& b) {
      return a[1] > b[1];
    });
    vector<Vec7i>::iterator i;
    int y1 = seed_line[1];
    int y2 = seed_line[3];
    int y1c;
    int y2c;
    for(i = lines.begin(); i != lines.end(); i++)
    {
        y1c = (*i)[1];
        y2c = (*i)[3];
        if(abs(y1 - y1c) < yThreshold1)
            continue;
        else if(abs(y2 - y1c) < yThreshold1)
            solid += 1;
        else
            dashed += 1;
        y1 = y1c;
        y2 = y2c;
    }
    if(solid > dashed || dashed == 0)
    {
        if(*status > -30)
            *status = *status - 1;
    }
    else
    {
        if(*status < 30)
            *status = *status + 1;
    }
}

void DecisionMaking::decideColor(vector<Vec7i> &lines, Mat &test, int side)
{
    int *status;
    int r = 0, g = 0, b = 0;
    int counter = 0;
    Vec3b colorVal;
    Point cen1, cen2;
    if (side == Decision::left)
        status = &(leftStatus[1]);
    else if (side == Decision::right)
        status = &(rightStatus[1]);
    if(lines.empty())
    {
        if(*status > 0)
            *status = *status - 1;
        else if (*status < 0)
            *status = *status + 1;
        return;
    }
    for (unsigned int i = 0; i < lines.size(); ++i)
    {
        if (lines[i][6] == USED)
            continue;
        lines[i][6] = USED;
        cen1 = Point((lines[i][0] + lines[i][2]) / 2, (lines[i][1] + lines[i][3]) / 2);
        for (unsigned int j = 0; j < lines.size(); ++j)
        {
            if (lines[j][6] == USED)
                continue;
            if (abs(lines[i][0] - lines[j][0]) <= xThreshold2 && abs(lines[i][2] - lines[j][2]) <= xThreshold2 &&
                    abs(lines[i][1] - lines[j][1]) <= yThreshold2 && abs(lines[i][3] - lines[j][3]) <= yThreshold2)
            {
                counter++;
                lines[j][6] = USED;
                cen2 = Point((lines[j][0] + lines[j][2]) / 2, (lines[j][1] + lines[j][3]) / 2);
                colorVal = test.at<Vec3b>(Point((cen1.x + cen2.x) / 2, (cen1.y + cen2.y) / 2));
                b += colorVal.val[0];
                g += colorVal.val[1];
                r += colorVal.val[2];
            }
        }
    }
    if (counter == 0)
        return;
    b /= counter;
    g /= counter;
    r /= counter;
    if(abs(b - g) <= 50 && abs(b - r) <= 50 && abs(r - g) <= 50)
    {
        if(*status > -30)
            *status = *status - 1;
    }
    else
    {
        if(*status < 30)
            *status = *status + 1;
    }
}

Vec2i DecisionMaking::getLeftStatus()
{
    Vec2i ls;
    // Type
    if(leftStatus[0] > 0)
        ls[0] = 1;
    else
        ls[0] = 0;
    // Color
    if(leftStatus[1] > 0)
        ls[1] = 0;
    else
        ls[1] = 1;
    return ls;
}

Vec2i DecisionMaking::getRightStatus()
{
    Vec2i rs;
    // Type
    if(rightStatus[0] > 0)
        rs[0] = 1;
    else
        rs[0] = 0;
    // Color
    if(rightStatus[1] > 0)
        rs[1] = 0;
    else
        rs[1] = 1;
    return rs;
}
