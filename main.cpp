#include "master.h"
#include "d_bus.h"

int main(int argc, char **argv)
{
    MASTER app(argc, argv);
    if (!QDBusConnection::sessionBus().isConnected()) {
        exit(1);
    }
    if (!QDBusConnection::sessionBus().registerService("com.stage.master")) {
        exit(1);
    }
    new D_BUS(&app);
    QDBusConnection::sessionBus().registerObject("/", &app);

    QString guiProcess = qApp->applicationDirPath() + "/../build-gui/gui";
    QProcess gui;
    QString streamProcess = qApp->applicationDirPath() + "/../build-stream/stream";
    QProcess stream;
    QString detectionProcess = qApp->applicationDirPath() + "/../build-detection/detection";
    QProcess detection;
    QString regProcess = qApp->applicationDirPath() + "/../build-reg/reg";
    QProcess reg;
    QSharedMemory smStreamGUI;
    QSharedMemory smStreamDetection;
    QSharedMemory smDetectionReg;

    QString keyId1 = app.createSharedMemorySection(smStreamGUI, SM_STREAM_GUI_SIZE, "stream", "gui");
    QString keyId2 = app.createSharedMemorySection(smStreamDetection, SM_STREAM_DETECTION_SIZE, "stream", "detection");
    QString keyId3 = app.createSharedMemorySection(smDetectionReg, SM_DETECTION_REG_SIZE, "detection", "reg");
    app.STREAM_GUI_KEY = keyId1;
    app.STREAM_DETECTION_KEY = keyId2;
    app.DETECTION_REG_KEY = keyId3;
    qint64 streamPID = app.createProcess(stream, streamProcess);
//    app.assignProcessToCore(streamPID, 5);
    qint64 guiPID = app.createProcess(gui, guiProcess);
//    app.assignProcessToCore(guiPID, 6);
    qint64 detectionPID = app.createProcess(detection, detectionProcess);
//    app.assignProcessToCore(detectionPID, 4);
    qint64 regPID = app.createProcess(reg, regProcess);
//    app.assignProcessToCore(regPID, 7);

    return app.exec();
}
