#ifndef D_BUS_H
#define D_BUS_H

#include <QtDBus>
#include "master.h"

class D_BUS : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.stage.master")
public:
    D_BUS(MASTER *app);

private:
    MASTER *app;

public slots:
    QString getSTREAMGUIKEY();

signals:
};

#endif // D_BUS_H
