#include <QApplication>
#include "mainwindow.h"
#include "back-end/pipeline.h"
#include "back-end/kalman.h"
#include "back-end/filter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Pipeline p;
    p.connectFrontEndToBackEnd(&w);

//    vector<Vec2i> points;
//    points = {
//        {401, 290},
//        {398, 293},
//        {394, 297},
//        {390, 302},
//        {385, 307},
//        {381, 312},
//        {376, 318},
//        {370, 325},
//        {364, 333},
//        {357, 342},
//        {349, 352},
//        {340, 364},
//        {330, 378},
//        {317, 395},
//        {303, 415},
//        {285, 441},
//        {262, 474},
//        {232, 519},
//        {190, 580},
//        {129, 672}
//    };




//    Kalman k;
//    k.kalmanFilter(points, 'l');

    vector<Vec4i> input_lines = {
            {337, 0, 339, 195},
            {354, 250, 347, 0},
            {130, 88, 123, 131},
            {133, 89, 133, 100},
            {138, 90, 137, 102},
            {133, 137, 140, 97},
            {122, 133, 128, 127},
            {130, 129, 133, 139},
            {120, 132, 123, 135},
            {128, 132, 124, 139},
            {338, 189, 379, 649},
            {393, 649, 354, 249},
            {114, 273, 105, 274},
            {105, 273, 98, 332},
            {117, 274, 112, 283},
            {111, 332, 116, 281},
            {96, 335, 104, 328},
            {106, 329, 112, 337},
            {103, 330, 105, 339},
            {86, 525, 91, 533},
            {96, 526, 91, 534},
            {83, 528, 78, 612},
            {92, 611, 96, 530},
            {77, 614, 85, 606},
            {87, 607, 94, 614},
            {85, 608, 85, 618},
            {379, 651, 389, 652},
            {368, 654, 381, 654},
            {392, 650, 389, 654},
            {392, 648, 394, 652},
            {384, 657, 418, 653},
            {0, 655, 373, 655},
            {416, 655, 478, 655},
            {378, 657, 385, 658}
        };
    Filter f;
    Mat hi = imread("/home/yousef/projects/real-time_lane_detection/Data/samplesFrames/ipm1.png");
    if (hi.empty())
        qDebug() << "empty mat";
    f.falseDetectionElimination(hi, input_lines);
    vector<Vec7i> *linesptr = f.getFilteredLines();

//    CurveFit c;
//    vector<Vec4i> llines;
//    vector<Vec4i> rlines;
//    vector<Vec2i> *lpts;
//    vector<Vec2i> *rpts;
//    llines.push_back(Vec4i{71, 626, 73, 546});
//    llines.push_back(Vec4i{57, 624, 60, 545});
//    llines.push_back(Vec4i{79, 349, 81, 298});
//    llines.push_back(Vec4i{66, 348, 68, 300});
//    llines.push_back(Vec4i{86, 161, 87, 123});
//    llines.push_back(Vec4i{75, 160, 76, 125});
//    rlines.push_back(Vec4i{364, 650, 262, 0});
//    rlines.push_back(Vec4i{378, 649, 273, 0});
//    c.fromLinesToPoints(llines, rlines);
//    c.setParameters(100, 754, 20);
//    c.doCurveFitting(CurveFitting::left_points);
//    c.doCurveFitting(CurveFitting::right_points);
//    lpts = c.getLeftPtsAfterFit();
//    rpts = c.getRightPtsAfterFit();
//    cv::Mat yousef = cv::imread("/home/yousef/projects/real-time_lane_detection/python_code/yousef.png");
//    for (int i = 0; i < lpts->size(); i++)
//        cv::circle(yousef, cv::Point(lpts->at(i)[0], lpts->at(i)[1]), 4, cv::Scalar(0, 0, 255), 1, CV_AA);
//    for (int i = 0; i < rpts->size(); i++)
//        cv::circle(yousef, cv::Point(rpts->at(i)[0], rpts->at(i)[1]), 4, cv::Scalar(0, 0, 255), 1, CV_AA);
//    cv::imwrite("yousef.png", yousef);


//    llines.clear();
//    rlines.clear();
//    llines.push_back(Vec4i{97, 623, 101, 541});
//    llines.push_back(Vec4i{84, 622, 88, 543});
//    llines.push_back(Vec4i{117, 340, 122, 285});
//    llines.push_back(Vec4i{105, 337, 111, 283});
//    rlines.push_back(Vec4i{345, 88, 348, 0});
//    rlines.push_back(Vec4i{371, 392, 354, 64});
//    rlines.push_back(Vec4i{353, 322, 345, 85});
//    rlines.push_back(Vec4i{357, 84, 358, 0});
//    c.fromLinesToPoints(llines, rlines);
//    c.setParameters(100, 754, 20);
//    c.doCurveFitting(CurveFitting::left_points);
//    c.doCurveFitting(CurveFitting::right_points);
//    lpts = c.getLeftPtsAfterFit();
//    rpts = c.getRightPtsAfterFit();
//    cv::Mat yousef1 = cv::imread("/home/yousef/projects/real-time_lane_detection/python_code/yousef.png");
//    for (int i = 0; i < lpts->size(); i++)
//        cv::circle(yousef1, cv::Point(lpts->at(i)[0], lpts->at(i)[1]), 4, cv::Scalar(0, 0, 255), 1, CV_AA);
//    for (int i = 0; i < rpts->size(); i++)
//        cv::circle(yousef1, cv::Point(rpts->at(i)[0], rpts->at(i)[1]), 4, cv::Scalar(0, 0, 255), 1, CV_AA);
//    cv::imwrite("yousef.png", yousef1);

    return a.exec();
}
