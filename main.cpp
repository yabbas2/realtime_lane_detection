#include <QCoreApplication>
#include "curvefit.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

using namespace chrono;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

/*************      load detection output from text file         ***********/

    Vec7i v;
    vector<Vec7i> vv;
    vector<vector<Vec7i>> vvvl;
    vector<vector<Vec7i>> vvvr;
    string line;
    string number;
    float fnum;
    int count = 0;
    bool left = true;
    ifstream myfile ("../regionGrowingOutput.txt");
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
                if(count >= 7)
                {
                    vv.push_back(v);
                    count = 0;
                }
            }
            if(left)
                vvvl.push_back(vv);
            else
                vvvr.push_back(vv);
            left = !left;
            vv.clear();
        }
        myfile.close();
    }
    else
        cout << "Unable to open file";
/******************************  end  **********************************/

    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    int counter = 0;
    int allTime = 0;


    vector<Vec2f> leftPoints;
    vector<Vec2f> rightPoints;
    CurveFit c;
    int ipmFrameRows = 854;
    c.setParameters(0, ipmFrameRows, 20);

    int size;
    if(vvvl.size() > vvvr.size())
        size = vvvl.size();
    else
        size = vvvr.size();
    for (int i = 0; i < size; ++i) {
        if (! c.validateLineBefore(vvvl[i], CurveFitting::left))
//            redetectLines(CurveFitting::left);
        c.doCurveFitting(CurveFitting::left);
        if (! c.validateLineBefore(vvvl[i], CurveFitting::right))
//            redetectLines(CurveFitting::right);
        c.doCurveFitting(CurveFitting::right);
        c.validateLineAfter();
        leftPoints = *(c.getPtsAfterFit(CurveFitting::left));
        rightPoints = *(c.getPtsAfterFit(CurveFitting::right));
    }
    return a.exec();
}
