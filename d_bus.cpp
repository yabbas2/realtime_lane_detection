#include "d_bus.h"

D_BUS::D_BUS(Track *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
}
