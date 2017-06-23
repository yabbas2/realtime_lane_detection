#ifndef MASTER_H
#define MASTER_H

#include <QApplication>
#include <QSharedMemory>
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QStringList>

#define SM_STREAM_GUI_SIZE          1152000

class MASTER : public QApplication
{
public:
    MASTER(int argc, char *argv[]);
    QString createSharedMemorySection(QSharedMemory &sm, int size, QString first, QString second);
    qint64 createProcess(QProcess &p, QString &program, QStringList &arguments);
    bool assignProcessToCore(qint64 &pid, int core);
};

#endif // MASTER_H
