#ifndef MASTER_H
#define MASTER_H

#include <QApplication>
#include <QSharedMemory>
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QStringList>
#include "../logger/logger.h"

#define SM_STREAM_GUI_SIZE                  1152000
#define SM_STREAM_DETECTION_SIZE            1152000
#define SM_DETECTION_REG_SIZE               1408

class MASTER : public QApplication
{
public:
    MASTER(int &argc, char **argv);
    QString createSharedMemorySection(QSharedMemory &sm, int size, QString first, QString second);
    qint64 createProcess(QProcess &p, QString &program);
    bool assignProcessToCore(qint64 &pid, int core);
    QString STREAM_GUI_KEY;
    QString STREAM_DETECTION_KEY;
    QString DETECTION_REG_KEY;

private:
    Logger log;
};

#endif // MASTER_H
