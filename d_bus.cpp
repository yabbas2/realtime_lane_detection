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

QString D_BUS::getSTREAMDETECTIONKEY()
{
    return app->STREAM_DETECTION_KEY;
}

QString D_BUS::getDETECTIONREGKEY()
{
    return app->DETECTION_REG_KEY;
}
