#include "d_bus.h"

D_BUS::D_BUS(Reg *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
}

void D_BUS::RGCFV()
{
    if (app->busy)
        return;
    app->process();
}
