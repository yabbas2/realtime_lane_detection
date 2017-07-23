#include "master.h"
#include "d_bus.h"
#include <unistd.h>

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
    qint64 guiPID;
    QString streamProcess = qApp->applicationDirPath() + "/../build-stream/stream";
    QProcess stream;
    qint64 streamPID;
    QString ipmProcess = qApp->applicationDirPath() + "/../build-ipm/ipm";
    QProcess ipm;
    qint64 ipmPID;
    QString detectionProcess = qApp->applicationDirPath() + "/../build-detection/detection";
    QProcess detection;
    qint64 detectionPID;
    QString regProcess = qApp->applicationDirPath() + "/../build-reg/reg";
    QProcess reg;
    qint64 regPID;
    QString trackProcess = qApp->applicationDirPath() + "/../build-track/track";
    QProcess track;
    qint64 trackPID;
    QSharedMemory smIPMStreamGUI;
    QSharedMemory smDetectionReg;
    QSharedMemory smRegTrack;
    QSharedMemory smGUITrack;
    QSharedMemory smGUIReg;
    QSharedMemory smIPMDetection;

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
    streamPID = app.createProcess(stream, streamProcess);
    app.assignProcessToCore(streamPID, 6);
    ipmPID = app.createProcess(ipm, ipmProcess);
    app.assignProcessToCore(ipmPID, 5);
    guiPID = app.createProcess(gui, guiProcess);
    app.assignProcessToCore(guiPID, 7);
    detectionPID = app.createProcess(detection, detectionProcess);
    app.assignProcessToCore(detectionPID, 3);
    regPID = app.createProcess(reg, regProcess);
    app.assignProcessToCore(regPID, 1);
    trackPID = app.createProcess(track, trackProcess);
    app.assignProcessToCore(trackPID, 2);

    return app.exec();
}
