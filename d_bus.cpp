#include "d_bus.h"

D_BUS::D_BUS(MASTER *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
}

QString D_BUS::getSTREAMGUIKEY()
{
    return app->STREAM_GUI_KEY;
}
