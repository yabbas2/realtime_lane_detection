#include "detection.h"

Detection::Detection(int &argc, char **argv) :
    QApplication(argc, argv)
{
    log.openFile(qApp->applicationDirPath() + "/../logger/logFiles/log_detection.txt", QIODevice::WriteOnly);
    log.write("----------------------------------------------------------");
    log.write("------------------------NEW RUN---------------------------");
    log.write("----------------------------------------------------------");
    ifMaster = new QDBusInterface("com.stage.master", "/", "com.stage.master", bus, this);
    ifReg = new QDBusInterface("com.stage.reg", "/", "com.stage.reg", bus2, this);
    QDBusReply<QString> key = ifMaster->call("getDETECTIONREGKEY");
    sm.setKey(key.value());
    sm.attach(QSharedMemory::ReadWrite);
    QDBusReply<QString> key2 = ifMaster->call("getSTREAMDETECTIONKEY");
    sm2.setKey(key2.value());
    sm2.attach(QSharedMemory::ReadOnly);
    frameCount = 0;
    lsd = createLineSegmentDetector(LSD_REFINE_STD);
    busy = false;
}

void Detection::lineSegmentDetector()
{
    busy = true;
    t1 = high_resolution_clock::now();
    frameCount++;
    while (!sm2.lock());
    sharedData2 *sData2 = (sharedData2*) sm2.data();
    inputFrame = Mat(frameWidth, frameHeight, CV_8UC3, sData2->ipmData);
    sm2.unlock();
    cvtColor(inputFrame, processedFrame, COLOR_BGR2GRAY);
    GaussianBlur(processedFrame, processedFrame, Size(9, 9), 1, 2);
    lsd->detect(processedFrame, lines);
    uchar *ipmData = inputFrame.data;
    while (!sm.lock());
    sharedData *sData = (sharedData*) sm.data();
    sData->actualSize = (lines.size() >= LINES_MAX_SIZE) ? LINES_MAX_SIZE : lines.size();
    for (int i = 0; i < sData->actualSize; ++i)
    {
        sData->lineSegments[i][0] = (int) lines[i][0];
        sData->lineSegments[i][1] = (int) lines[i][1];
        sData->lineSegments[i][2] = (int) lines[i][2];
        sData->lineSegments[i][3] = (int) lines[i][3];
    }
    memcpy(sData->ipmData, ipmData, IPM_FRAME_SIZE);
    sm.unlock();
    ifReg->call("FRGCFV");
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    log.write("[DETECTION] frame no. " + QString::number(frameCount) + ", exec time: " + QString::number(duration));
    busy = false;
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
