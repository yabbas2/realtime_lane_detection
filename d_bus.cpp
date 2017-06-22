#include "d_bus.h"

D_BUS::D_BUS(GUI *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
    connect(app->mainWindow.vidWid, SIGNAL(pauseStreaming()), this, SIGNAL(pauseStream()));
    connect(app->mainWindow.vidWid, SIGNAL(startStreaming()), this, SIGNAL(startStream()));
    connect(app->mainWindow.sidebar->inputMethod, SIGNAL(pauseStreaming()), this, SIGNAL(pauseStream()));
    connect(app->mainWindow.sidebar->inputMethod, SIGNAL(startStreaming()), this, SIGNAL(startStream()));
    connect(app->mainWindow.sidebar->inputMethod, SIGNAL(changeVideoSource(QString)), this, SIGNAL(setVideoSource(QString)));
}


void D_BUS::initViewers()
{
    app->mainWindow.initViewers();
}
