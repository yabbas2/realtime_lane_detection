#include "processing.hpp"

using namespace cv;

Mat processing::roiImg;

void processing::determineROI() {
    Rect roi(0, normalFrame.rows/2, normalFrame.cols, normalFrame.rows/2);
    normalFrame = normalFrame(roi);
}
