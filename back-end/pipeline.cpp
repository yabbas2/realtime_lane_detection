#include "pipeline.h"

Pipeline::Pipeline()
{
    streamObj = new Stream;
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
}

void Pipeline::exec()
{
    normalFrame = streamObj->getFrame();
    if (normalFrame.empty())
        return;

    ipmObj->transform(normalFrame, "sample1");
    ipmFrame = ipmObj->getIPMFrame();

    streamObj->setIPMFrame(ipmFrame);

    lineDetector->lineSegmentDetector(*ipmFrame);
    detectedLines = lineDetector->getDetectedLines();


    filter->falseDetectionElimination(*ipmFrame, *detectedLines);
    filteredLines = filter->getFilteredLines();

    regGrow->regionGrowing(*filteredLines, ipmFrame->cols);
    leftRegion = regGrow->getLeftRegion();
    rightRegion = regGrow->getRightRegion();
    leftSeedLine = regGrow->getLeftSeedLine();
    rightSeedLine = regGrow->getRightSeedLine();

    if (curveFit->fromLinesToPoints(*leftRegion, *rightRegion))
    {
        curveFit->setParameters(100, ipmFrame->rows - 100, 20);
        curveFit->doCurveFitting(CurveFitting::left_points);
        curveFit->doCurveFitting(CurveFitting::right_points);
        leftPoints = curveFit->getLeftPtsAfterFit();
        rightPoints = curveFit->getRightPtsAfterFit();
    }
    else
    {
        leftPoints = &emptyPoints;
        rightPoints = &emptyPoints;
    }

    decisionMake->decide(*leftRegion, *leftSeedLine, Decision::left_region);
    decisionMake->decide(*rightRegion, *rightSeedLine, Decision::right_region);
    if (decisionMake->getLeftStatus())
        qDebug() << "left is dashed";
    else
        qDebug() << "left is solid";
    if (decisionMake->getRightStatus())
        qDebug() << "right is dashed";
    else
        qDebug() << "right is solid";

    *leftPoints = ipmObj->inverseTransformL(*leftPoints);
//    leftPoints = ipmObj->getFinalPoints();
    *rightPoints = ipmObj->inverseTransformR(*rightPoints);
//    rightPoints = ipmObj->getFinalPoints();

    k->kalmanFilter(*leftPoints, kalman::left_region);
    k->kalmanFilter(*rightPoints, kalman::right_region);
    leftPoints = k->getPrevLeftPoints();
    rightPoints = k->getPrevRightPoints();

    streamObj->setPointsToDraw(*leftPoints, *rightPoints);
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
    streamObj->setViewers(multiViewer, fsViewer);
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
