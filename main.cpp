#include <QCoreApplication>
#include "decisionmaking.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

using namespace chrono;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    int counter = 0;
    int allTime = 0;


    return a.exec();
}
