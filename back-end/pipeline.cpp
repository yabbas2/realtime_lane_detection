#include "pipeline.h"

Pipeline::Pipeline()
{
//    streamObj = new Stream;
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
    magdy = Mat::zeros(ipmFrame->rows, ipmFrame->cols, CV_8UC3);

//    streamObj->setIPMFrame(ipmFrame);

    lineDetector->lineSegmentDetector(*ipmFrame);
    detectedLines = lineDetector->getDetectedLines();

    _3ebs = filter->falseDetectionElimination(*ipmFrame, *detectedLines);
    streamObj->setIPMFrame(ipmFrame);
    filteredLines = filter->getFilteredLines();
    for (unsigned int i = 0; i < filteredLines->size(); ++i)
        line(magdy, Point((int)filteredLines->at(i)[0], (int)filteredLines->at(i)[1]), Point((int)filteredLines->at(i)[2], (int)filteredLines->at(i)[3]),
                Scalar(255, 255, 255), 1, LINE_AA);

    regGrow->regionGrowing(*filteredLines, ipmFrame->cols);
    leftRegion = regGrow->getLeftRegion();
    rightRegion = regGrow->getRightRegion();
    leftSeedLine = regGrow->getLeftSeedLine();
    rightSeedLine = regGrow->getRightSeedLine();
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

    if (curveFit->fromLinesToPoints(*leftRegion, CurveFitting::left_points))
    {
        curveFit->setParameters(0, ipmFrame->rows, 20);
        curveFit->doCurveFitting(CurveFitting::left_points);
        leftPoints = curveFit->getLeftPtsAfterFit();
        if (leftPoints->at(0)[0] < -1000 || leftPoints->at(0)[0] > 1000||  leftPoints->at(19)[0] < -1000 || leftPoints->at(19)[0] > 1000 )
            leftPoints = &emptyPoints;
    }
    else
        leftPoints = &emptyPoints;

     if (curveFit->fromLinesToPoints(*rightRegion, CurveFitting::right_points))
     {
         curveFit->setParameters(0, ipmFrame->rows, 20);
         curveFit->doCurveFitting(CurveFitting::right_points);
         rightPoints = curveFit->getRightPtsAfterFit();
         if (rightPoints->at(0)[0] < -1000 || rightPoints->at(0)[0] > 1000||  rightPoints->at(19)[0] < -1000 || rightPoints->at(19)[0] > 1000 )
             rightPoints = &emptyPoints;
     }
     else
         rightPoints = &emptyPoints;

    vector<Vec2i> left;
    vector<Vec2i> right;
    for (unsigned int i = 0; i < leftPoints->size(); i++)
        left.push_back(Vec2i{(int)leftPoints->at(i)[0], (int)leftPoints->at(i)[1]});
    for (unsigned int i = 0; i < rightPoints->size(); i++)
        right.push_back(Vec2i{(int)rightPoints->at(i)[0], (int)rightPoints->at(i)[1]});
    polylines(magdy, left, false, Scalar(0, 255, 255), 1, LINE_AA);
    polylines(magdy, right, false, Scalar(0, 255, 255), 1, LINE_AA);

    streamObj->setIPMBW(&magdy);

    decisionMake->decide(*leftRegion, *leftSeedLine, Decision::left_region);
    decisionMake->decide(*rightRegion, *rightSeedLine, Decision::right_region);
    if (decisionMake->getLeftStatus() > 0)
        qDebug() << "left is dashed";
    else if(decisionMake->getLeftStatus() < 0)
        qDebug() << "left is solid";
    if (decisionMake->getRightStatus() > 0)
        qDebug() << "right is dashed";
    else if(decisionMake->getRightStatus() < 0)
        qDebug() << "right is solid";

    ipmObj->inverseTransform(*leftPoints, ipm::left_points);
    leftPoints = ipmObj->getLeftPoints();
    ipmObj->inverseTransform(*rightPoints, ipm::right_points);
    rightPoints = ipmObj->getRightPoints();

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
