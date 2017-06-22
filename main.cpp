#include <QApplication>
#include "stream.h"
#include "d_bus.h"

int main(int argc, char *argv[])
{
    Stream app(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "[STREAM] cannot connect to D-Bus - exiting..";
        return 1;
    }

    if (!QDBusConnection::sessionBus().registerService("com.stage.stream")) {
        qDebug() << "[STREAM] cannot register service";
        exit(1);
    }

//    Stream st;

    new D_BUS(&app);

    QDBusConnection::sessionBus().registerObject("/", &app);

//    app.changeStreamInSource("/home/yousef/projects/real-time_lane_detection/datasets/youtube/youtube_video1.mp4");
//    st.startStream();

    return app.exec();
}
