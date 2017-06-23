#include "master.h"

MASTER::MASTER(int &argc, char **argv) :
    QApplication(argc, argv)
{
}

QString MASTER::createSharedMemorySection(QSharedMemory &sm, int size, QString first, QString second)
{
    QString keyName = "SM." + first + "." + second + ".1";
    sm.setKey(keyName);
    while (! sm.create(size, QSharedMemory::ReadWrite))
    {
        if (sm.error() == QSharedMemory::AlreadyExists)
            keyName += "." + QString::number((rand() % 10) + 1);
        else
            exit(1);
        sm.setKey(keyName);
    }
    return keyName;
}

qint64 MASTER::createProcess(QProcess &p, QString &program)
{
    p.start(program);
    p.waitForStarted();
    if (p.state() == QProcess::Running)
        qDebug() << "[MASTER]" << program << "is running with pid:" << QString::number(p.processId());
    else
        qDebug() << "[MASTER] failed to start" << program;
    return p.processId();
}

bool MASTER::assignProcessToCore(qint64 &pid, int core)
{
    QString program = "taskset";
    QStringList args;
    args << "-pc" << QString::number(core) << QString::number(pid);
    if (QProcess::startDetached(program, args))
    {
        qDebug() << "[MASTER] assigned" << QString::number(pid) << "to core" << QString::number(core);
        return true;
    }
    else
    {
        qDebug() << "[MASTER] failed to assign" << QString::number(pid) << "to core" << QString::number(core);
        return false;
    }
}
