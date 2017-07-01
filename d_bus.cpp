#include "d_bus.h"

D_BUS::D_BUS(IPM *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
}

void D_BUS::setVideoName(QString s)
{
    app->setVideoPts(s);
}

void D_BUS::ipmTransform()
{
    if (app->busy)
        return;
    app->ipmTransform();
}
