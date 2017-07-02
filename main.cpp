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
    QString ipmProcess = qApp->applicationDirPath() + "/../build-ipm/ipm";
    QProcess ipm;
    QString detectionProcess = qApp->applicationDirPath() + "/../build-detection/detection";
    QProcess detection;
    QString regProcess = qApp->applicationDirPath() + "/../build-reg/reg";
    QProcess reg;
    QString trackProcess = qApp->applicationDirPath() + "/../build-track/track";
    QProcess track;
    QSharedMemory smIPMStreamGUI;
    QSharedMemory smDetectionReg;
    QSharedMemory smRegTrack;
    QSharedMemory smGUITrack;
    QSharedMemory smGUIReg;
    QSharedMemory smIPMDetection;
    QStringList guiArgs;
    guiArgs << "-platform" << "linuxfb";

    QString keyId1 = app.createSharedMemorySection(smIPMStreamGUI, SM_STREAM_GUI_IPM_SIZE, "stream_ipm", "gui_ipm");
    QString keyId2 = app.createSharedMemorySection(smIPMDetection, SM_IPM_DETECTION_SIZE, "ipm", "detection");
    QString keyId3 = app.createSharedMemorySection(smDetectionReg, SM_DETECTION_REG_SIZE, "detection", "reg");
    QString keyId4 = app.createSharedMemorySection(smRegTrack, SM_REG_TRACK_SIZE, "reg", "track");
    QString keyId5 = app.createSharedMemorySection(smGUITrack, SM_GUI_TRACK_SIZE, "gui", "track");
    QString keyId6 = app.createSharedMemorySection(smGUIReg, SM_GUI_REG_SIZE, "gui", "reg");
    app.STREAM_GUI_IPM_KEY = keyId1;
    app.IPM_DETECTION_KEY = keyId2;
    app.DETECTION_REG_KEY = keyId3;
    app.REG_TRACK_KEY = keyId4;
    app.GUI_TRACK_KEY = keyId5;
    app.GUI_REG_KEY = keyId6;
    qint64 streamPID = app.createProcess(stream, streamProcess);
//    app.assignProcessToCore(streamPID, 5);
    qint64 ipmPID = app.createProcess(ipm, ipmProcess);
//    app.assignProcessToCore(ipmPID, 5);
    qint64 guiPID = app.createProcess(gui, guiProcess, guiArgs);
//    app.assignProcessToCore(guiPID, 6);
    qint64 detectionPID = app.createProcess(detection, detectionProcess);
//    app.assignProcessToCore(detectionPID, 4);
    qint64 regPID = app.createProcess(reg, regProcess);
//    app.assignProcessToCore(regPID, 7);
    qint64 trackPID = app.createProcess(track, trackProcess);
//    app.assignProcessToCore(trackPID, 7);

    return app.exec();
}
