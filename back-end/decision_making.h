#ifndef DECISION_MAKING_H
#define DECISION_MAKING_H

#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>

#define yThreshold  150

using namespace cv;
using namespace std;

typedef Vec<int,7> Vec7i;

namespace Decision {
enum {left_region, right_region};
}

class DecisionMaking
{
private:
    int dashed;
    int solid;
    bool isDashedLeft;
    bool isDashedRight;
    bool *isDashedPtr;
public:
    DecisionMaking();
    void decide(vector<Vec7i> &lines, Vec7i &seed_line, int side);
    bool getLeftStatus();
    bool getRightStatus();
};

#endif // DECISION_MAKING_H
