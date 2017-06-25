#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QIODevice>
#include <QString>

class Logger
{
public:
    explicit Logger();
    ~Logger();
    void setFile(QString fileName);
    void write(QString text);

private:
    QFile *file;
};

#endif // LOGGER_H
