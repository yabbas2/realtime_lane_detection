#include "d_bus.h"

D_BUS::D_BUS(Stream *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
}

void D_BUS::startStream()
{
    app->startStream();
}

void D_BUS::stopStream()
{
    app->pauseStream();
}

void D_BUS::setStreamSource(QString source)
{
    app->changeStreamInSource(source);
}

void D_BUS::setVideoName(QString s)
{
    app->ipm.setVideoPts(s);
}
