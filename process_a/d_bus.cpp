#include "d_bus.h"

D_BUS::D_BUS(MASTER *application) :
    QDBusAbstractAdaptor(application),
    app(application)
{
}

QString D_BUS::getIPMSTREAMGUIKEY()
{
    return app->STREAM_GUI_IPM_KEY;
}

QString D_BUS::getDETECTIONREGKEY()
{
    return app->DETECTION_REG_KEY;
}

QString D_BUS::getREGTRACKKEY()
{
    return app->REG_TRACK_KEY;
}

QString D_BUS::getGUITRACKKEY()
{
    return app->GUI_TRACK_KEY;
}

QString D_BUS::getGUIREGKEY()
{
    return app->GUI_REG_KEY;
}

QString D_BUS::getIPMDETECTIONKEY()
{
    return app->IPM_DETECTION_KEY;
}
