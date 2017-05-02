#ifndef DECISION_MAKING_H
#define DECISION_MAKING_H

#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class DecisionMaking
{
private:
    float yThreshold;
    int dashed;
    int solid;
    bool isDashed;
public:
    DecisionMaking();
    void decide(vector<Vec4i> &lines, vector<float> &seed_line);
    bool getDecision();
};

#endif // DECISION_MAKING_H
