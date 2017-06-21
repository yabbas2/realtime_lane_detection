#include <QApplication>
#include "mainwindow.h"
#include "stream.h"
#include "viewers/fullscreenvideoviewer.h"
#include "viewers/multivideoviewer.h"
#include "sidebar/side_bar.h"
#include "videowidget.h"
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Stream st;
    st.connectToFrontEnd(&w);
    st.start();

    return a.exec();
}
