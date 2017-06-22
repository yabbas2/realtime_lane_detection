#include <QApplication>
#include "gui.h"
#include "d_bus.h"

int main(int argc, char *argv[])
{
    GUI gui(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "[GUI] cannot connect to D-Bus - exiting..";
        return 1;
    }

    if (!QDBusConnection::sessionBus().registerService("com.stage.gui")) {
        qDebug() << "[GUI] cannot register service";
        exit(1);
    }

    new D_BUS(&gui);

    QDBusConnection::sessionBus().registerObject("/", &gui);

    gui.mainWindow.show();

    return gui.exec();
}
