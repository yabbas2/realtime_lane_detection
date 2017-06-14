#include "pipeline.h"

Pipeline::Pipeline()
{
    ipmObj = new IPM;
    lineDetector = new LineDetection;
    filter = new Filter;
    regGrow = new RegionGrowing;
    curveFit = new CurveFit;
    k = new Kalman;
    decisionMake = new DecisionMaking;
    timer = new QTimer(this);
    emptyPoints.clear();
    connect(timer, SIGNAL(timeout()), this, SLOT(exec()));
    leftStatus[0] = 0;
    leftStatus[1] = 1;
    rightStatus[0] = 0;
    rightStatus[1] = 1;
}

void Pipeline::exec()
{
    normalFrame = streamObj->getFrame();
    if (normalFrame.empty())
        return;

    ipmObj->transform(normalFrame, videoName);
    ipmFrame = ipmObj->getIPMFrame();
    magdy = Mat::zeros(ipmFrame->rows, ipmFrame->cols, CV_8UC3);

//    streamObj->setIPMFrame(ipmFrame);

    lineDetector->lineSegmentDetector(*ipmFrame);
    detectedLines = lineDetector->getDetectedLines();

    _3ebs = filter->falseDetectionElimination(*ipmFrame, *detectedLines);
//    streamObj->setIPMFrame(&_3ebs);
    filteredLines = filter->getFilteredLines();
    for (unsigned int i = 0; i < filteredLines->size(); ++i)
        line(magdy, Point((int)filteredLines->at(i)[0], (int)filteredLines->at(i)[1]), Point((int)filteredLines->at(i)[2], (int)filteredLines->at(i)[3]),
                Scalar(255, 255, 255), 1, LINE_AA);

//    filter->falseDetectionElimination(*ipmFrame, *_amany_);
//    filteredLines = filter->getFilteredLines();
//    for (size_t i = 0; i < _amany_->size(); ++i)
//        line(*ipmFrame, Point((int)_amany_->at(i)[0], (int)_amany_->at(i)[1]), Point((int)_amany_->at(i)[2], (int)_amany_->at(i)[3])
//                ,Scalar(255, 0, 0), 3);
    streamObj->setIPMFrame(ipmFrame);

    regGrow->regionGrowing(*filteredLines, ipmFrame->cols, RegGrow::both);
    leftRegion = regGrow->getLeftRegion();
    rightRegion = regGrow->getRightRegion();
    leftSeedLine = regGrow->getLeftSeedLine();
    rightSeedLine = regGrow->getRightSeedLine();

    curveFit->setParameters(0, ipmFrame->rows, 20);
    if (! curveFit->validateLineBefore(*leftRegion, CurveFitting::left))
        redetectLines(CurveFitting::left);
    curveFit->doCurveFitting(CurveFitting::left);
    if (! curveFit->validateLineBefore(*rightRegion, CurveFitting::right))
        redetectLines(CurveFitting::right);
    curveFit->doCurveFitting(CurveFitting::right);
    curveFit->validateLineAfter();
    leftPoints = curveFit->getPtsAfterFit(CurveFitting::left);
    rightPoints = curveFit->getPtsAfterFit(CurveFitting::right);

    if (!leftRegion->empty())
        for (unsigned int i = 0; i < leftRegion->size(); ++i)
            line(magdy, Point((int)leftRegion->at(i)[0], (int)leftRegion->at(i)[1]), Point((int)leftRegion->at(i)[2], (int)leftRegion->at(i)[3]),
                    Scalar(0, 255, 0), 1, LINE_AA);
    if (!rightRegion->empty())
        for (unsigned int i = 0; i < rightRegion->size(); ++i)
            line(magdy, Point((int)rightRegion->at(i)[0], (int)rightRegion->at(i)[1]), Point((int)rightRegion->at(i)[2], (int)rightRegion->at(i)[3]),
                    Scalar(0, 0, 255), 1, LINE_AA);
    line(magdy, Point((int)leftSeedLine->operator [](0), (int)leftSeedLine->operator [](1)), Point((int)leftSeedLine->operator [](2), (int)leftSeedLine->operator [](3)),
         Scalar(255, 0, 255), 1, LINE_AA);
    line(magdy, Point((int)rightSeedLine->operator [](0), (int)rightSeedLine->operator [](1)), Point((int)rightSeedLine->operator [](2), (int)rightSeedLine->operator [](3)),
         Scalar(255, 0, 255), 1, LINE_AA);

    vector<Vec2i> left;
    vector<Vec2i> right;
    for (unsigned int i = 0; i < leftPoints->size(); i++)
    {
        circle(magdy, Point((int)leftPoints->at(i)[0], (int)leftPoints->at(i)[1]), 4,
                Scalar(0, 127, 255));
        left.push_back(Vec2i{(int)leftPoints->at(i)[0], (int)leftPoints->at(i)[1]});
    }
    for (unsigned int i = 0; i < rightPoints->size(); i++)
    {
        circle(magdy, Point((int)rightPoints->at(i)[0], (int)rightPoints->at(i)[1]), 4,
                Scalar(0, 127, 255));
        right.push_back(Vec2i{(int)rightPoints->at(i)[0], (int)rightPoints->at(i)[1]});
    }
    polylines(magdy, left, false, Scalar(0, 255, 255), 1, LINE_AA);
    polylines(magdy, right, false, Scalar(0, 255, 255), 1, LINE_AA);

    streamObj->setIPMBW(&magdy);

    decisionMake->decideType(*leftRegion, *leftSeedLine, Decision::left);
    decisionMake->decideType(*rightRegion, *rightSeedLine, Decision::right);
    decisionMake->decideColor(*leftRegion, *ipmFrame, Decision::left);
    decisionMake->decideColor(*rightRegion, *ipmFrame, Decision::right);
    leftStatus = decisionMake->getLeftStatus();
    rightStatus = decisionMake->getRightStatus();

    ipmObj->inverseTransform(*leftPoints, ipm::left);
    leftPoints = ipmObj->getPoints(ipm::left);
    ipmObj->inverseTransform(*rightPoints, ipm::right);
    rightPoints = ipmObj->getPoints(ipm::right);


    k->kalmanFilter(*leftPoints, kalman::left);
    k->kalmanFilter(*rightPoints, kalman::right);
    leftPoints = k->getPrevPoints(kalman::left);
    rightPoints = k->getPrevPoints(kalman::right);


    streamObj->setInfo(*leftPoints, *rightPoints, leftStatus, rightStatus);
}

void Pipeline::redetectLines(int side)
{
    vector<Vec4f> *_amany_;
    lineDetector->cannyHough(*ipmFrame, side);
    _amany_ = lineDetector->getTmpLines(side);
    filter->falseDetectionElimination(*ipmFrame, *_amany_);
    filteredLines = filter->getFilteredLines();
    regGrow->regionGrowing(*filteredLines, ipmFrame->cols, side);
    if (side == CurveFitting::left)
    {
        leftRegion = regGrow->getLeftRegion();
        leftSeedLine = regGrow->getLeftSeedLine();
        curveFit->validateLineBefore(*leftRegion, CurveFitting::left);
    }
    else if (side == CurveFitting::right)
    {
        rightRegion = regGrow->getRightRegion();
        rightSeedLine = regGrow->getRightSeedLine();
        curveFit->validateLineBefore(*rightRegion, CurveFitting::right);
    }
}

void Pipeline::connectFrontEndToBackEnd(MainWindow *w)
{
    multiViewer = w->getMultiVideoViewerWidget();
    fsViewer = w->getFullScreenVideoViewerWidget();
    videoWidget = w->getVideoWidget();
    sideBar = w->getSideBarWidget();
    connect(videoWidget, SIGNAL(pauseStreaming()), this, SLOT(pause_timers()));
    connect(videoWidget, SIGNAL(startStreaming()), this, SLOT(start_timers()));
    connect(sideBar->inputMethod, SIGNAL(pauseStreaming()), this, SLOT(pause_timers()));
    connect(sideBar->inputMethod, SIGNAL(startStreaming()), this, SLOT(start_timers()));
    connect(sideBar->inputMethod, SIGNAL(changeVideoSource(QString)), streamObj, SLOT(changeStreamInSource(QString)));
    connect(sideBar->inputMethod, SIGNAL(reInit()), this, SLOT(initStatus()));
    connect(sideBar->inputMethod, SIGNAL(setVideoName(QString)), this, SLOT(setVideoSource(QString)));
    streamObj->setViewers(multiViewer, fsViewer);
}

void Pipeline::setVideoSource(QString s)
{
    videoName = s;
}

void Pipeline::connectToStreamEngine(Stream *s)
{
    streamObj = s;
}

void Pipeline::initStatus()
{
    timer->stop();
    streamObj->reInitStream();
}

void Pipeline::pause_timers()
{
    timer->stop();
    streamObj->pause_timers();
}

void Pipeline::start_timers()
{
    streamObj->start_timers();
    timer->start(static_cast<int> (1000/streamObj->fps) + delayOffset);
}

Pipeline::~Pipeline()
{

}
