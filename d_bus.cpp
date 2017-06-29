#include "d_bus.h"

D_BUS::D_BUS(Track *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
}

void D_BUS::track()
{
    if (app->busy)
        return;
    app->process();
}
