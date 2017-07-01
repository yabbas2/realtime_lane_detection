#include "d_bus.h"

D_BUS::D_BUS(GUI *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
}


void D_BUS::initViewers()
{
    app->mainWindow.initViewers();
}

void D_BUS::showFrames()
{
    app->mainWindow.showFrames();
}

void D_BUS::setData()
{
    app->mainWindow.updateData();
}
