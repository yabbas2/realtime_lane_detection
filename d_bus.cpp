#include "d_bus.h"

D_BUS::D_BUS(Detection *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
}

void D_BUS::detect()
{
    if (app->busy)
        return;
    app->process();
}
