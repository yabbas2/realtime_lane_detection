#include "d_bus.h"

D_BUS::D_BUS(Stream *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
    connect(app, SIGNAL(initScreens()), this, SIGNAL(initState()));
}

void D_BUS::startStream()
{
    app->startStream();
}

void D_BUS::stopStream()
{
    app->pauseStream();
}
