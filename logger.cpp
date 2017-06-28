#include "logger.h"

Logger::Logger()
{
}

void Logger::openFile(QString fileName, QIODevice::OpenMode om)
{
    file = new QFile;
    file->setFileName(fileName);
    file->open(om | QIODevice::Text);
}

void Logger::write(QString text)
{
    text = "[ " + QDateTime::currentDateTime().toString("dd.MM.yyyy || hh:mm:ss") + " ] => " + text + "\n";
    QTextStream out(file);
    out.setCodec("UTF-8");
    out << text;
}

Logger::~Logger()
{
    file->close();
}
