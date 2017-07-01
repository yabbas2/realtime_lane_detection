#ifndef MASTER_H
#define MASTER_H

#include <QApplication>
#include <QSharedMemory>
#include <QProcess>
#include <QString>
#include <QStringList>
#include "../logger/logger.h"

#define SM_STREAM_GUI_IPM_SIZE              1152000
#define SM_IPM_DETECTION_SIZE               1152000
#define SM_DETECTION_REG_SIZE               1204
#define SM_REG_TRACK_SIZE                   328
#define SM_GUI_TRACK_SIZE                   328
#define SM_GUI_REG_SIZE                     16

class MASTER : public QApplication
{
public:
    MASTER(int &argc, char **argv);
    QString createSharedMemorySection(QSharedMemory &sm, int size, QString first, QString second);
    qint64 createProcess(QProcess &p, QString &program);
    bool assignProcessToCore(qint64 &pid, int core);
    QString STREAM_GUI_IPM_KEY;
    QString DETECTION_REG_KEY;
    QString REG_TRACK_KEY;
    QString GUI_TRACK_KEY;
    QString GUI_REG_KEY;
    QString IPM_DETECTION_KEY;

private:
    Logger log;
};

#endif // MASTER_H
