#include "track.h"

Track::Track(int &argc, char **argv) :
    QApplication(argc, argv)
{
    busy = true;
    log.openFile(qApp->applicationDirPath() + "/../logger/logFiles/log_track.txt", QIODevice::WriteOnly);
    log.write("----------------------------------------------------------");
    log.write("------------------------NEW RUN---------------------------");
    log.write("----------------------------------------------------------");
    ifMaster = new QDBusInterface("com.stage.master", "/", "com.stage.master", bus, this);
    ifGUI = new QDBusInterface("com.stage.gui", "/", "com.stage.gui", bus2, this);
    QDBusReply<QString> key = ifMaster->call("getGUITRACKKEY");
    sm.setKey(key.value());
    sm.attach(QSharedMemory::ReadWrite);
    QDBusReply<QString> key2 = ifMaster->call("getREGTRACKKEY");
    sm2.setKey(key2.value());
    sm2.attach(QSharedMemory::ReadOnly);
    initFilter();
    isMeasure = 0;
    frameCount = 0;
    failCount = 0;
    mp = Mat::zeros(2, 1, CV_64F);
    busy = false;
}

void Track::process()
{
    busy = true;
    t1 = high_resolution_clock::now();
    frameCount++;
    inputLeftPts.clear();
    inputRightPts.clear();
    while (!sm2.lock());
    sharedData *sData2 = (sharedData*) sm2.data();
    for (int i = 0; i < sData2->actualLeftSize; ++i)
        inputLeftPts.push_back(Vec2f{sData2->leftPts[i][0], sData2->leftPts[i][1]});
    for (int i = 0; i < sData2->actualRightSize; ++i)
        inputRightPts.push_back(Vec2f{sData2->rightPts[i][0], sData2->rightPts[i][1]});
    sm2.unlock();
    kalmanFilter(TRACK::left);
    if (failCount <= 30)
        smooth();
    kalmanFilter(TRACK::right);
    if (failCount <= 30)
        smooth();
    inverseTransform(TRACK::left);
    inverseTransform(TRACK::right);
    while (!sm.lock());
    sharedData *sData = (sharedData*) sm.data();
    sData->actualLeftSize = outputLeftPts.size();
    for (int i = 0; i < sData->actualLeftSize; ++i)
    {
        sData->leftPts[i][0] = outputLeftPts[i][0];
        sData->leftPts[i][1] = outputLeftPts[i][1];
    }
    sData->actualRightSize = outputRightPts.size();
    for (int i = 0; i < sData->actualRightSize; ++i)
    {
        sData->rightPts[i][0] = outputRightPts[i][0];
        sData->rightPts[i][1] = outputRightPts[i][1];
    }
    sm.unlock();
    ifGUI->call("setData");
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    log.write("[TRACK] frame no. " + QString::number(frameCount) + ", exec time: " + QString::number(duration));
    busy = false;
}

void Track::inverseTransform(int side)
{
    vector<Vec2f> *outputPts;
    vector<Vec2f> *inputPts;
    if (side == TRACK::left)
    {
        inputPts = &prevLeftPts;
        outputPts = &outputLeftPts;
    }
    else if (side == TRACK::right)
    {
        inputPts = &prevRightPts;
        outputPts = &outputRightPts;
    }
    if(inputPts->empty())
    {
        outputPts->clear();
        return;
    }
    float z;
    float ptx, pty;
    outputPts->clear();
    for(unsigned int it = 0; it < inputPts->size(); it++){
        z = 1 / (inverseHomography.at<double>(2,0) * inputPts->at(it)[0] + inverseHomography.at<double>(2,1) * inputPts->at(it)[1]
                + inverseHomography.at<double>(2,2));
        ptx =  ((inverseHomography.at<double>(0,0) * inputPts->at(it)[0] + inverseHomography.at<double>(0,1) * inputPts->at(it)[1]
                + inverseHomography.at<double>(0,2)) * z);
        pty =  ((inverseHomography.at<double>(1,0) * inputPts->at(it)[0] + inverseHomography.at<double>(1,1) * inputPts->at(it)[1]
                + inverseHomography.at<double>(1,2)) * z);
        outputPts->push_back(Vec2f{ptx, pty});
    }
}

void Track::setInvMat(QString videoName)
{
    while (busy);
    busy = true;
    Mat inPts;
    Mat dstPts;
    dstPts = (Mat_<double>(4,2) << 0, 0, frameHeight, 0, frameHeight, frameWidth, 0, frameWidth);
    if (videoName == "youtube_video1")
        inPts = (Mat_<double>(4, 2) << 325, 280, 485, 280, 748, 478, 120, 478);
    else if (videoName == "youtube_video2" || videoName == "youtube_video3")
        inPts = (Mat_<double>(4, 2) << 340, 280, 520, 280, 770, 478, 120, 478);
    else if (videoName == "youtube_video4")
        inPts = (Mat_<double>(4, 2) << 310, 280, 445, 280, 615, 390, 200, 390);
    else if (videoName == "youtube_video5")
        inPts = (Mat_<double>(4, 2) << 365, 300, 490, 300, 700, 440, 210, 440);
    else if (videoName == "youtube_video6")
        inPts = (Mat_<double>(4, 2) << 380, 220, 480, 220, 610, 350, 235, 350);
    else if (videoName.startsWith("kitti_video"))
        inPts = (Mat_<double>(4, 2) << 330, 300, 460, 300, 600, 478, 200, 478);
    else if (videoName.startsWith("udacity_video"))
        inPts = (Mat_<double>(4, 2) << 340, 310, 475, 310, 740, 430, 120, 430);
    inPts.convertTo(inPts, CV_32F);
    dstPts.convertTo(dstPts, CV_32F);
    inverseHomography = getPerspectiveTransform(dstPts, inPts);
    busy = false;
}

void Track::initFilter()
{
    for (int i = 0; i < 20; ++i) {
        leftKalman[i] = KalmanFilter(4, 2, 0, CV_64F);
        leftKalman[i].measurementMatrix = (Mat_ <double>(2, 4) << 1, 0, 0, 0, 0, 1, 0, 0);
        leftKalman[i].transitionMatrix = (Mat_ <double>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
        leftKalman[i].processNoiseCov = (Mat_ <double>(4, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) * 0.0003;
        rightKalman[i] = KalmanFilter(4, 2, 0, CV_64F);
        rightKalman[i].measurementMatrix = (Mat_ <double>(2, 4) << 1, 0, 0, 0, 0, 1, 0, 0);
        rightKalman[i].transitionMatrix = (Mat_ <double>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
        rightKalman[i].processNoiseCov = (Mat_ <double>(4, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) * 0.0003;
    }
}

void Track::kalmanFilter(int side)
{
    if (side == TRACK::left)
    {
        inputPts = &inputLeftPts;
        newPts = &newLeftPts;
        prevPts = &prevLeftPts;
        k = leftKalman;
    }
    else if (side == TRACK::right)
    {
        inputPts = &inputRightPts;
        newPts = &newRightPts;
        prevPts = &prevRightPts;
        k = rightKalman;
    }
    if(inputPts->empty())
    {
        *newPts = *prevPts;
        failCount++;
    }
    else
    {
        *newPts = *inputPts;
        failCount = 0;
    }
}

void Track::smooth()
{
    prevPts->clear();
    if (newPts->empty())
        return;
    if(isMeasure < 2)
    {
        isMeasure++;
        for (int i = 0; i < PTS_NUM; ++i) {
            mp.at<double>(0, 0) = newPts->at(i)[0];
            mp.at<double>(1, 0) = newPts->at(i)[1];
            for (int j = 0; j < 50; ++j) {
                k[i].correct(mp);
                tp = k[i].predict();
            }
        }
    }
    for (int i = 0; i < PTS_NUM; ++i) {
        mp.at<double>(0, 0) = newPts->at(i)[0];
        mp.at<double>(1, 0) = newPts->at(i)[1];
        k[i].correct(mp);
        tp = k[i].predict();
        prevPts->push_back(Vec2f{(float)tp.at<double>(0, 0), (float)tp.at<double>(1, 0)});
    }
}
