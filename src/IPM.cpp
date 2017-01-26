#include "Processing.hpp"

using namespace cv;

Mat Processing::ipmFrame;
Mat Processing::HomographyToOriginal;

void Processing::fourPointTransform()
{
    int height = normalFrame.rows;
    int width = normalFrame.cols;
    Mat HomographyToInv = getPerspectiveTransform(pts,dst);
    HomographyToOriginal = getPerspectiveTransform(dst, pts);
    warpPerspective(normalFrame, ipmFrame, HomographyToInv, Size(height, width));
}
void Processing::inverse()
{
    unsigned int size = (unsigned int)detectedLanes.size();
    for(unsigned int i = 0; i < size; i++)
    {
        double Z, px1, py1, px2, py2;
        Vec4d x = detectedLanes.front();
        detectedLanes.pop_front();
        Z = 1 / (HomographyToOriginal.at<double>(2,0) * x[0] + HomographyToOriginal.at<double>(2,1) * x[1] + HomographyToOriginal.at<double>(2,2));
        px1 =  ((HomographyToOriginal.at<double>(0,0) * x[0] + HomographyToOriginal.at<double>(0,1) * x[1] + HomographyToOriginal.at<double>(0,2)) * Z);
        py1 =  ((HomographyToOriginal.at<double>(1,0) * x[0] + HomographyToOriginal.at<double>(1,1) * x[1] + HomographyToOriginal.at<double>(1,2)) * Z);
        Z = 1 / (HomographyToOriginal.at<double>(2,0) * x[2] + HomographyToOriginal.at<double>(2,1) * x[3] + HomographyToOriginal.at<double>(2,2));
        px2 =  ((HomographyToOriginal.at<double>(0,0) * x[2] + HomographyToOriginal.at<double>(0,1) * x[3] + HomographyToOriginal.at<double>(0,2)) * Z);
        py2 =  ((HomographyToOriginal.at<double>(1,0) * x[2] + HomographyToOriginal.at<double>(1,1) * x[3] + HomographyToOriginal.at<double>(1,2)) * Z);

        detectedLanes.push_back(Vec4d{px1, py1, px2, py2});
        assert(detectedLanes.size() == size);
    }
}
