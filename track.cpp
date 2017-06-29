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
    sharedData2 *sData2 = (sharedData2*) sm2.data();
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
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    log.write("[TRACK] frame no. " + QString::number(frameCount) + ", exec time: " + QString::number(duration));
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
        for (int i = 0; i < 20; ++i) {
            mp.at<double>(0, 0) = newPts->at(i)[0];
            mp.at<double>(1, 0) = newPts->at(i)[1];
            for (int j = 0; j < 50; ++j) {
                k[i].correct(mp);
                tp = k[i].predict();
            }
        }
    }
    for (int i = 0; i < 20; ++i) {
        mp.at<double>(0, 0) = newPts->at(i)[0];
        mp.at<double>(1, 0) = newPts->at(i)[1];
        k[i].correct(mp);
        tp = k[i].predict();
        prevPts->push_back(Vec2f{(float)tp.at<double>(0, 0), (float)tp.at<double>(1, 0)});
    }
}
