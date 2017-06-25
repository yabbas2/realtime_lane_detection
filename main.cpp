#include "stream.h"
#include "d_bus.h"

int main(int argc, char **argv)
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
    new D_BUS(&app);
    QDBusConnection::sessionBus().registerObject("/", &app);
    return app.exec();
}
