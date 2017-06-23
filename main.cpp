#include <QCoreApplication>
#include "filter.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

using namespace chrono;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


/*************      load detection output from text file         ***********/

    Vec4f v;
    vector<Vec4f> vv;
    vector<vector<Vec4f>> vvv;
    string line;
    string number;
    float fnum;
    int count = 0;

    ifstream myfile ("../detectionOutput.txt");
    if (myfile.is_open())
    {
        while( getline(myfile, line) )
        {
            for (int i = 0; i < line.size(); ++i) {
                number = "";
                while(i < line.size() && line[i] != '\t')
                    number += line[i++];
                fnum = stof(number);
                v[count++] = fnum;
                if(count >= 4)
                {
                    vv.push_back(v);
                    count = 0;
                }
            }
            vvv.push_back(vv);
            vv.clear();
        }
        myfile.close();
    }
    else
        cout << "Unable to open file";
/******************************  end  **********************************/

/********************* nshof el ipmFrame da b3deen ***********************/
    string filename = "/home/meegz/Projects/Processes/ipmOutput.mp4";
    VideoCapture capture(filename);
    capture.open(filename);
    Mat ipmFrame;
    if( !capture.isOpened() )
        throw "Error when reading steam_avi";
/********************* *************************** ***********************/

    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    int counter = 0;
    int allTime = 0;

    Filter f;
    vector<vector<Vec7i>> out;
    for (int i = 0; i < vvv.size(); ++i) {
        capture >> ipmFrame;
        t1 = high_resolution_clock::now();

        f.falseDetectionElimination(ipmFrame, vvv[i]);

        t2 = high_resolution_clock::now();

        out.push_back(*(f.getFilteredLines()));

        auto duration = duration_cast<milliseconds>(t2 - t1).count();
        allTime += duration;
        counter++;

    }
    cout << "Avg Exec Time: " << allTime/counter << "ms" << endl;


/*************      save filtered lines into text file         ***********/

//    vector<Vec7i> tmp;
//    ofstream ofs ("../filterOutput.txt", ofstream::out);

//    for (int i = 0; i < out.size(); ++i) {
//        tmp = out[i];
//        for (int ii = 0; ii < tmp.size(); ++ii)
//            for (int iii = 0; iii < 7; ++iii)
//            {
//                ofs << tmp[ii][iii];
//                ofs << '\t';
//            }
//        if(i != out.size() -1)
//            ofs << '\n';
//    }
//    ofs.close();


    return a.exec();
}
