#ifndef D_BUS_H
#define D_BUS_H

#include <QtDBus>
#include "stream.h"

class D_BUS : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.stage.stream")
public:
    D_BUS(Stream *app);

private:
    Stream *app;

public slots:
    Q_NOREPLY void startStream();
    Q_NOREPLY void stopStream();
    Q_NOREPLY void setStreamSource(QString);
    Q_NOREPLY void setVideoName(QString);
};

#endif // D_BUS_H
