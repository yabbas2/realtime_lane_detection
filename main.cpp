#include <QApplication>
#include "mainwindow.h"
#include "back-end/stream.h"
#include "back-end/curvefit.h"
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Stream st;
    st.connectFrontEndToStreamBackEnd(&w);

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
