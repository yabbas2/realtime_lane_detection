#include "processing.hpp"

using namespace cv;

Mat processing::ipmFrame;
Mat processing::HomographyToOriginal;
vector<int> processing::arrow;

void processing::fourPointTransform()
{
    int height = normalFrame.rows;
    int width = normalFrame.cols;
    Mat HomographyToInv = getPerspectiveTransform(pts,dst);
    HomographyToOriginal = getPerspectiveTransform(dst, pts);
    warpPerspective(normalFrame, ipmFrame, HomographyToInv, Size(height, width));
}
void processing::inverse()
{
    unsigned int size = (unsigned int)detectedLanes.size();
    int height = normalFrame.rows;
    double Z, px1, py1, px2, py2;
    float slope;
    vector<float> X2Points(size);
    vector<float> X1Points(size);
    for(unsigned int i = 0; i < size; i++)
    {
        Vec4d x = detectedLanes.front();
        detectedLanes.pop_front();
        Z = 1 / (HomographyToOriginal.at<double>(2,0) * x[0] + HomographyToOriginal.at<double>(2,1) * x[1] + HomographyToOriginal.at<double>(2,2));
        px1 =  ((HomographyToOriginal.at<double>(0,0) * x[0] + HomographyToOriginal.at<double>(0,1) * x[1] + HomographyToOriginal.at<double>(0,2)) * Z);
        py1 =  ((HomographyToOriginal.at<double>(1,0) * x[0] + HomographyToOriginal.at<double>(1,1) * x[1] + HomographyToOriginal.at<double>(1,2)) * Z);
        Z = 1 / (HomographyToOriginal.at<double>(2,0) * x[2] + HomographyToOriginal.at<double>(2,1) * x[3] + HomographyToOriginal.at<double>(2,2));
        px2 =  ((HomographyToOriginal.at<double>(0,0) * x[2] + HomographyToOriginal.at<double>(0,1) * x[3] + HomographyToOriginal.at<double>(0,2)) * Z);
        py2 =  ((HomographyToOriginal.at<double>(1,0) * x[2] + HomographyToOriginal.at<double>(1,1) * x[3] + HomographyToOriginal.at<double>(1,2)) * Z);

        slope = (float)(py2 - py1) / (float)(px2 - px1);
        px2 = (height - py1) / slope + px1;
        py2 = height;

        X2Points[i] = (float)px2;
        X1Points[i] = (float)px1;

        detectedLanes.push_back(Vec4f{(float)px1, (float)py1, (float)px2, (float)py2});
    }
    CV_Assert(detectedLanes.size() == size);
    if(size > 5 || size == 0) return;
    float max = 0;
    unsigned int index = 0;
    for(unsigned int i = 0; i < size-1; i++)
    {
        if(X2Points[i] < 0 || X2Points[i+1] < 0)
            continue;
        if((X2Points[i] - X2Points[i+1]) < max)
        {
            max = X2Points[i] - X2Points[i+1];
            index = i;
        }
    }
    if(max == 0) return;
    arrow.clear();
    arrow.push_back((int)((X1Points[index]+X1Points[index+1])/2));
    arrow.push_back(height-50);
    arrow.push_back((int)((X1Points[index]+X1Points[index+1])/2));
    arrow.push_back(height-100);
 }
