#include "decision_making.h"

bool DecisionMaking::isDashed(vector<Vec4f> lines, vector<float> seed_line)
{
    if(lines.empty() || seed_line.empty())
        return false;
    yThreshold = 150.0;
    dashed = 0;
    solid = 0;
    sort(lines.begin(), lines.end(),
              [](const std::vector<float>& a, const std::vector<float>& b) {
      return a[1] > b[1];
    });
    vector<Vec4f>::iterator i;
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
        if(solid > dashed || dashed == 0)
            return false;
        else
            return true;
    }
}
