#ifndef D_BUS_H
#define D_BUS_H

#include <QtDBus>
#include "detection.h"

class D_BUS : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.stage.detection")
public:
    D_BUS(Detection *app);

private:
    Detection *app;

public slots:
    Q_NOREPLY void detect();
};

#endif // D_BUS_H
