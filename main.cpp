#include <QCoreApplication>
#include "regiongrowing.h"
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
    vector<vector<Vec7i>> vvv;
    string line;
    string number;
    float fnum;
    int count = 0;

    ifstream myfile ("../filterOutput.txt");
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
            vvv.push_back(vv);
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

    RegionGrowing r;
    vector<Vec7i> leftRegion;
    vector<Vec7i> rightRegion;
    vector<vector<Vec7i>> out;
    int width = 480;
    for (int i = 0; i < vvv.size(); ++i) {

        t1 = high_resolution_clock::now();

        r.regionGrowing(vvv[i], width, RegGrow::both);

        t2 = high_resolution_clock::now();

        leftRegion = *(r.getLeftRegion());
        rightRegion = *(r.getRightRegion());
        out.push_back(leftRegion);
        out.push_back(rightRegion);

        auto duration = duration_cast<milliseconds>(t2 - t1).count();
        allTime += duration;
        counter++;

    }

    cout << "Avg Exec Time: " << allTime/counter << "ms" << endl;





/*************      save filtered lines into text file         ***********/

//        vector<Vec7i> tmp;
//        ofstream ofs ("../regionGrowingOutput.txt", ofstream::out);

//        for (int i = 0; i < out.size(); ++i) {
//            tmp = out[i];
//            for (int ii = 0; ii < tmp.size(); ++ii)
//                for (int iii = 0; iii < 7; ++iii)
//                {
//                    ofs << tmp[ii][iii];
//                    ofs << '\t';
//                }
//            if(i != out.size() -1)
//                ofs << '\n';
//        }
//        ofs.close();


    return a.exec();
}
