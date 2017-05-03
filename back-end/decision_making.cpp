#include "decision_making.h"


DecisionMaking::DecisionMaking()
{
    leftStatus = 0;
    rightStatus = 0;
}

void DecisionMaking::decide(vector<Vec7i> &lines, Vec7i &seed_line, int side)
{
    int* status;
    if (side == Decision::left_region)
        status = &leftStatus;
    else if (side == Decision::right_region)
        status = &rightStatus;
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
        if(abs(y1 - y1c) < yThreshold)
            continue;
        else if(abs(y2 - y1c) < yThreshold)
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
        if(*status < 30)
            *status = *status + 1;
}

int DecisionMaking::getLeftStatus()
{
    if(leftStatus > 0)
        return 1;
    else if(leftStatus < 0)
        return -1;
    else
        return 0;
}

int DecisionMaking::getRightStatus()
{
    if(rightStatus > 0)
        return 1;
    else if(rightStatus < 0)
        return -1;
    else
        return 0;
}
