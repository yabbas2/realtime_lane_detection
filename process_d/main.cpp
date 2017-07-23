#include "detection.h"
#include "d_bus.h"

int main(int argc, char **argv)
{
    Detection app(argc, argv);
    if (!QDBusConnection::sessionBus().isConnected()) {
        exit(1);
    }
    if (!QDBusConnection::sessionBus().registerService("com.stage.detection")) {
        exit(1);
    }
    new D_BUS(&app);
    QDBusConnection::sessionBus().registerObject("/", &app);
    return app.exec();
}










