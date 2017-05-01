#include "decision_making.h"


DecisionMaking::DecisionMaking()
{
    yThreshold = 150.0;
}

void DecisionMaking::decide(vector<Vec4i> &lines, vector<float> &seed_line)
{
    if(lines.empty() || seed_line.empty())
    {
        isDashed = false;
        return;
    }
    dashed = 0;
    solid = 0;
    sort(lines.begin(), lines.end(),
              [](const Vec4i& a, const Vec4i& b) {
      return a[1] > b[1];
    });
    vector<Vec4i>::iterator i;
    float y1 = seed_line[1];
    float y2 = seed_line[3];
    float y1c;
    float y2c;
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
        isDashed = false;
    else
        isDashed = true;
}

bool DecisionMaking::getDecision()
{
    return isDashed;
}
