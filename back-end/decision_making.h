#ifndef DECISION_MAKING_H
#define DECISION_MAKING_H

#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>

#define yThreshold1     150
#define yThreshold2     10
#define xThreshold2     100
#define USED            1

using namespace cv;
using namespace std;

typedef Vec<int,7> Vec7i;

namespace Decision {
enum {left, right};
}

class DecisionMaking
{
private:
    Vec2i leftStatus;
    Vec2i rightStatus;
public:
    DecisionMaking();
    void decideType(vector<Vec7i> &lines, Vec7i &seed_line, int side);
    void decideColor(vector<Vec7i> &lines, Mat &test, int side);
    Vec2i getLeftStatus();
    Vec2i getRightStatus();
};

#endif // DECISION_MAKING_H
