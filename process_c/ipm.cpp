#include "ipm.h"

IPM::IPM(int &argc, char **argv) :
    QApplication(argc, argv)
{
    busy = true;
    log.openFile(qApp->applicationDirPath() + "/../logger/logFiles/log_ipm.txt", QIODevice::WriteOnly);
    log.write("----------------------------------------------------------");
    log.write("------------------------NEW RUN---------------------------");
    log.write("----------------------------------------------------------");
    ifMaster = new QDBusInterface("com.stage.master", "/", "com.stage.master", bus, this);
    ifDetection = new QDBusInterface("com.stage.detection", "/", "com.stage.detection", bus2, this);
    QDBusReply<QString> key = ifMaster->call("getIPMDETECTIONKEY");
    sm.setKey(key.value());
    sm.attach(QSharedMemory::ReadWrite);
    QDBusReply<QString> key2 = ifMaster->call("getIPMSTREAMGUIKEY");
    sm2.setKey(key2.value());
    sm2.attach(QSharedMemory::ReadOnly);
    frameCount = 0;
    busy = false;
}

void IPM::setVideoPts(QString videoName)
{
    busy = true;
    Mat inputPts;
    Mat dstPts;
    dstPts = (Mat_<double>(4,2) << 0, 0, frameHeight, 0, frameHeight, frameWidth, 0, frameWidth);
    if (videoName == "youtube_video1")
        inputPts = (Mat_<double>(4, 2) << 325, 280, 485, 280, 748, 478, 120, 478);
    else if (videoName == "youtube_video2" || videoName == "youtube_video3")
        inputPts = (Mat_<double>(4, 2) << 340, 280, 520, 280, 770, 478, 120, 478);
    else if (videoName == "youtube_video4")
        inputPts = (Mat_<double>(4, 2) << 310, 280, 445, 280, 615, 390, 200, 390);
    else if (videoName == "youtube_video5")
        inputPts = (Mat_<double>(4, 2) << 365, 300, 490, 300, 700, 440, 210, 440);
    else if (videoName == "youtube_video6")
        inputPts = (Mat_<double>(4, 2) << 380, 220, 480, 220, 610, 350, 235, 350);
    else if (videoName.startsWith("kitti_video"))
        inputPts = (Mat_<double>(4, 2) << 330, 300, 460, 300, 600, 478, 200, 478);
    else if (videoName.startsWith("udacity_video"))
        inputPts = (Mat_<double>(4, 2) << 340, 310, 475, 310, 740, 430, 120, 430);
    else
        return;
    inputPts.convertTo(inputPts, CV_32F);
    dstPts.convertTo(dstPts, CV_32F);
    transformHomography = getPerspectiveTransform(inputPts, dstPts);
    busy = false;
}

void IPM::ipmTransform()
{
    busy = true;
    t1 = high_resolution_clock::now();
    frameCount++;
    while (!sm2.lock());
    sharedData2 *sData2 = (sharedData2*) sm2.data();
    inputFrame = Mat(frameHeight, frameWidth, CV_8UC3, sData2->rawImg);
    sm2.unlock();
    warpPerspective(inputFrame, ipmFrame, transformHomography, Size(frameHeight, frameWidth));
    uchar *ipmData = ipmFrame.data;
    while (!sm.lock());
    sharedData *sData = (sharedData*) sm.data();
    memcpy(sData->ipmData, ipmData, IPM_FRAME_SIZE);
    sm.unlock();
    ifDetection->call("detect");
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    log.write("[IPM] frame no. " + QString::number(frameCount) + ", exec time: " + QString::number(duration));
    busy = false;
}
