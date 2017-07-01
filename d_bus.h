#ifndef D_BUS_H
#define D_BUS_H

#include <QtDBus>
#include "ipm.h"

class D_BUS : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.stage.ipm")
public:
    D_BUS(IPM *app);

private:
    IPM *app;

public slots:
    Q_NOREPLY void setVideoName(QString);
    Q_NOREPLY void ipmTransform();
};

#endif // D_BUS_H
