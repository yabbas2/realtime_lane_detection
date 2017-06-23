#include "d_bus.h"

D_BUS::D_BUS(MASTER *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
}
