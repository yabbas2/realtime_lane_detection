#include "gui.h"
#include "d_bus.h"

int main(int argc, char **argv)
{
    GUI gui(argc, argv);
    if (!QDBusConnection::sessionBus().isConnected()) {
        exit(1);
    }
    if (!QDBusConnection::sessionBus().registerService("com.stage.gui")) {
        exit(1);
    }
    new D_BUS(&gui);
    QDBusConnection::sessionBus().registerObject("/", &gui);
    gui.mainWindow.show();
    return gui.exec();
}
