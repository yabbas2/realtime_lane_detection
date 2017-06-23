#include <QCoreApplication>
#include "linedetection.h"
#include <QDebug>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace chrono;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    string filename = "/home/meegz/Projects/Processes/ipmOutput.mp4";
    VideoCapture capture(filename);
    capture.open(filename);
    Mat ipmFrame;
    if( !capture.isOpened() )
        throw "Error when reading steam_avi";

    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    int counter = 0;
    int allTime = 0;
    LineDetection l;
    int type;
    vector<vector<Vec4f>> v;
    cout << "1 for lineSegmentDetector .. else for cannyHough" << endl;
    cin >> type;
    if(type == 1)
        while(true)
        {
            t1 = high_resolution_clock::now();

            capture >> ipmFrame;
            if(ipmFrame.empty())
                break;
            l.lineSegmentDetector(ipmFrame);
            t2 = high_resolution_clock::now();

            v.push_back(*(l.getDetectedLines()));

            auto duration = duration_cast<milliseconds>(t2 - t1).count();
            allTime += duration;
            counter++;
        }
    else
        while(true)
        {
            t1 = high_resolution_clock::now();

            capture >> ipmFrame;
            if(ipmFrame.empty())
                break;
            l.lineSegmentDetector(ipmFrame);
            l.cannyHough(ipmFrame, LineDetector::right);
            l.cannyHough(ipmFrame, LineDetector::left);
            t2 = high_resolution_clock::now();

            v.push_back(*(l.getTmpLines(LineDetector::left)));
            v.push_back(*(l.getTmpLines(LineDetector::right)));

            auto duration = duration_cast<milliseconds>(t2 - t1).count();
            allTime += duration;
            counter++;
        }
    if(type == 1)
        cout << "Avg Exec Time for lineSegmentDetector: " << allTime/counter << "ms" << endl;
    else
        cout << "Avg Exec Time for cannyHough for both right and left: " << allTime/counter << "ms" << endl;

/*************      save output into text file         ***********/

//    vector<Vec4f> tmp;
//    ofstream ofs ("../detectionOutput.txt", ofstream::out);

//    for (int i = 0; i < v.size(); ++i) {
//        tmp = v[i];
//        for (int ii = 0; ii < tmp.size(); ++ii)
//            for (int iii = 0; iii < 4; ++iii)
//            {
//                ofs << tmp[ii][iii];
//                ofs << '\t';
//            }
//        if(i != v.size() -1)
//            ofs << '\n';
//    }
//    ofs.close();

    return a.exec();
}










