#ifndef D_BUS_H
#define D_BUS_H

#include <QtDBus>
#include <QString>
#include "gui.h"

class D_BUS : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.stage.gui")
public:
    D_BUS(GUI *app);

private:
    GUI *app;

public slots:
    void initViewers();

signals:
    void pauseStream();
    void startStream();
    void setVideoSource(QString);
};

#endif // D_BUS_H
