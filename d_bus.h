#ifndef D_BUS_H
#define D_BUS_H

#include <QtDBus>
#include "track.h"

class D_BUS : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.stage.track")
public:
    D_BUS(Track *app);

private:
    Track *app;

public slots:
    Q_NOREPLY void track();
};

#endif // D_BUS_H
