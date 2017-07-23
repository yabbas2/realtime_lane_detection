#ifndef D_BUS_H
#define D_BUS_H

#include <QtDBus>
#include "reg.h"

class D_BUS : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.stage.reg")
public:
    D_BUS(Reg *app);

private:
    Reg *app;

public slots:
    Q_NOREPLY void RGCFV();
};

#endif // D_BUS_H
