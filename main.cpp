#include "mainwindow.h"
#include <QApplication>
#include "back-end/stream.h"
#include <QThread>
//#include "front-end/videostatistics.h"
//#include "front-end/videocontrols.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Stream st;
    st.connectFrontEndToStreamBackEnd(&w);

    return a.exec();
}
