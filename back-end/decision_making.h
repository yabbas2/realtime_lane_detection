#ifndef DECISION_MAKING_H
#define DECISION_MAKING_H

#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;

class DecisionMaking
{
private:
    float yThreshold;
    int dashed;
    int solid;
public:
    bool isDashed(vector<Vec4f> lines, vector<float> seed_line);
};

#endif // DECISION_MAKING_H
