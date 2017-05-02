#include "decision_making.h"


DecisionMaking::DecisionMaking()
{

}

void DecisionMaking::decide(vector<Vec7i> &lines, Vec7i &seed_line, int side)
{
    if (side == Decision::left_region)
        isDashedPtr = &isDashedLeft;
    else if (side == Decision::right_region)
        isDashedPtr = &isDashedRight;
    if(lines.empty())
    {
        *isDashedPtr = false;
        return;
    }
    dashed = 0;
    solid = 0;
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
        else if(abs(y2 - y2c) < yThreshold)
            solid += 1;
        else
            dashed += 1;
        y1 = y1c;
        y2 = y2c;
    }
    if(solid > dashed || dashed == 0)
        *isDashedPtr = false;
    else
        *isDashedPtr = true;
}

bool DecisionMaking::getLeftStatus()
{
    return isDashedLeft;
}

bool DecisionMaking::getRightStatus()
{
    return isDashedRight;
}
