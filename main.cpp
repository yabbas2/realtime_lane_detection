#include "mainwindow.h"
#include <QApplication>
#include "back-end/stream.h"
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Stream st;
    st.connectToFrontEnd(&w);
    st.changeStreamInSource("/home/yousef/projects/real-time_lane_detection/Data/samplesVideos/sample1.mp4");

    return a.exec();
}
