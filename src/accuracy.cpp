#include "processing.hpp"

double Processing::error[3] = {0,0,0};
unsigned int Processing::mismatch = 0;
double Processing::accuracy = 0;

void Processing::calAccuracy()
{
    double fixedPoints[] = {14, 188, 350};
    auto it = detectedLanes.begin();
    if((int)detectedLanes.size() <= 3)
        for (int i = 0; i < (int)detectedLanes.size(); ++i) {
            if ((*it)[2] != (*it)[2])
            {
                mismatch++;
                it++;
            }
            else {
                if ((int) detectedLanes.size() == 3)
                    error[i] += abs((*it)[2] - fixedPoints[i]);
                else
                    error[i + 1] += abs((*it)[2] - fixedPoints[i]);
                it++;
            }
        }
}
void Processing::printAccuracy() {
    for (int i = 0; i < 3; ++i){
        Processing::error[i] /= Processing::framesNumber;
        accuracy += error[i];
    }
    cout << "Average error per line detected= " << accuracy/3 << " pixels" << endl;
    cout << "number of mismatched lines= " << mismatch << endl;
    cout << "percentage of mismatched lines= " << mismatch*100/(framesNumber*3) << "%" << endl;
}