#include "master.h"
#include "d_bus.h"


QString USER = qgetenv("USER");
QString guiProcess = "/home/" + USER + "/gui";
QProcess gui;
QString streamProcess = "/home/" + USER + "/stream";
QProcess stream;
QSharedMemory smStreamGUI;

int main(int argc, char *argv[])
{
    MASTER app(argc, argv);
    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "[MASTER] cannot connect to D-Bus - exiting..";
        return 1;
    }
    if (!QDBusConnection::sessionBus().registerService("com.stage.master")) {
        qDebug() << "[MASTER] cannot register service";
        exit(1);
    }
    new D_BUS(&app);
    QDBusConnection::sessionBus().registerObject("/", &app);

    QString keyId1 = app.createSharedMemorySection(smStreamGUI, SM_STREAM_GUI_SIZE,"stream", "gui");
    QStringList args1;
    args1 << keyId1;
    qint64 streamPID = app.createProcess(stream, streamProcess, args1);
    app.assignProcessToCore(streamPID, 5);
    qint64 guiPID = app.createProcess(gui, guiProcess, args1);
    app.assignProcessToCore(guiPID, 6);
    return app.exec();
}
