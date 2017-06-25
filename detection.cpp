#include "detection.h"

Detection::Detection(int &argc, char **argv) :
    QApplication(argc, argv)
{
    boundary_min = {75, 100, 0};
    boundary_max = {112, 225, 154};
    ifMaster = new QDBusInterface("com.stage.master", "/", "com.stage.master", bus, this);
    QDBusReply<QString> key = ifMaster->call("getDETECTIONREGKEY");
    sm.setKey(key.value());
    sm.attach(QSharedMemory::ReadWrite);
    QDBusReply<QString> key2 = ifMaster->call("getSTREAMDETECTIONKEY");
    sm2.setKey(key2.value());
    sm2.attach(QSharedMemory::ReadOnly);
    busy = false;
}

void Detection::lineSegmentDetector()
{
    busy = true;
    sm2.lock();
    sharedData2 *sData2 = (sharedData2*) sm2.data();
    inputFrame = Mat(frameWidth, frameHeight, CV_8UC3, sData2->ipmData);
    sm2.unlock();
    lsd = createLineSegmentDetector(LSD_REFINE_STD);
    cvtColor(inputFrame, processedFrame, COLOR_BGR2GRAY);
    GaussianBlur(processedFrame, processedFrame, Size(9, 9), 1, 2);
    lsd->detect(processedFrame, lines);
    filterPhaseOne();
    filterPhaseTwo();
    sm.lock();
    sharedData *sData = (sharedData*) sm.data();
    sData->actualSize = (phaseTwoFiltered.size() >= LINES_MAX_SIZE) ? LINES_MAX_SIZE : phaseTwoFiltered.size();
    for (int i = 0; i < sData->actualSize; ++i)
        for (int j = 0; j < 7; ++j)
            sData->lineSegments[i][j] = phaseTwoFiltered[i][j];
    sm.unlock();
    busy = false;
}

void Detection::filterPhaseOne()
{
    phaseOneFiltered.clear();
    for(unsigned int it = 0; it < lines.size(); ++it)
    {
        float x1 = lines[it][0], y1 = lines[it][1], x2 = lines[it][2], y2 = lines[it][3];
        if(y1 < y2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        float theta = fastAtan2((y2 - y1), (x2 - x1));
        if (theta > 180)
            theta -= 180;
        float length = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
        if ((threshold_angle_min < theta) && (theta < threshold_angle_max) && (length >= threshold_length))
            phaseOneFiltered.push_back(Vec7i{(int)x1, (int)y1, (int)x2, (int)y2, (int)theta, (int)length, 0});
    }
}
void Detection::filterPhaseTwo()
{
    phaseTwoFiltered.clear();
    vector<vector<Point>> contours;
    cvtColor(inputFrame, processedFrame, COLOR_BGR2HSV);
    inRange(processedFrame, boundary_min, boundary_max, processedFrame);
    findContours(processedFrame, contours, 1, 2);
    if(contours.empty())
    {
        phaseTwoFiltered = phaseOneFiltered;
        return;
    }
    for(unsigned int it_lines = 0; it_lines < phaseOneFiltered.size(); ++it_lines)
    {
        int flag = 0;
        for(unsigned int it_contours = 0; it_contours < contours.size(); ++it_contours)
        {
            if (contourArea(contours[it_contours]) > 10000 || contourArea(contours[it_contours]) < 1000)
                continue;
            Point2f a((float)phaseOneFiltered[it_lines][0], (float)phaseOneFiltered[it_lines][1]);
            Point2f b((float)phaseOneFiltered[it_lines][2], (float)phaseOneFiltered[it_lines][3]);
            double dist1 = pointPolygonTest(contours[it_contours], a, true);
            double dist2 = pointPolygonTest(contours[it_contours], b, true);
            if(dist1 >= -17 || dist2 >= -17)
            {
                flag = 1;
                break;
            }
        }
        if(!flag)
            phaseTwoFiltered.push_back(Vec7i{phaseOneFiltered[it_lines][0], phaseOneFiltered[it_lines][1], phaseOneFiltered[it_lines][2],
                                  phaseOneFiltered[it_lines][3], phaseOneFiltered[it_lines][4], phaseOneFiltered[it_lines][5],
                                  phaseOneFiltered[it_lines][6]});
    }
}

void Detection::cannyHough(int side)
{
    Mat edgeImage;
    int xThreshold = inputFrame.cols / 2;
    vector<Vec4i> tmpLines;
    Canny(inputFrame, edgeImage, 50, 70);
    HoughLinesP(edgeImage, tmpLines, 1, CV_PI/180, 50, 50, 70);
    if (side == detectionSide::left)
    {
        tmpLeftLines.clear();
        for (size_t i = 0; i < tmpLines.size(); ++i)
            if (tmpLines[i][0] <= xThreshold && tmpLines[i][2] <= xThreshold)
                tmpLeftLines.push_back(Vec4f{(float)tmpLines[i][0], (float)tmpLines[i][1], (float)tmpLines[i][2], (float)tmpLines[i][3]});
    }
    if (side == detectionSide::right)
    {
        tmpRightLines.clear();
        for (size_t i = 0; i < tmpLines.size(); ++i)
            if (tmpLines[i][0] > xThreshold && tmpLines[i][2] > xThreshold)
                tmpRightLines.push_back(Vec4f{(float)tmpLines[i][0], (float)tmpLines[i][1], (float)tmpLines[i][2], (float)tmpLines[i][3]});
    }
}
