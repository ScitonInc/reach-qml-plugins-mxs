#include "systemplugin.h"
#include "beeper.h"
#include "backlight.h"
#include "network.h"
#include "system.h"
#include "context2d.h"
#include "canvas.h"
#include "gpiopininput.h"
#include "gpiopinoutput.h"
#include "gpiopinsinput.h"
#include "gpiopinsoutput.h"
#include "sqlite.h"
#include "upgrade.h"
#include <QtDeclarative>

void SystemPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("com.reachtech.systemplugin"));
    qmlRegisterType<System>(uri, 1, 0, "System");
    qmlRegisterType<Beeper>(uri, 1, 0, "Beeper");
    qmlRegisterType<Backlight>(uri, 1, 0, "Backlight");
    qmlRegisterType<Network>(uri, 1, 0, "Network");
    qmlRegisterType<Canvas>(uri, 1, 0, "Canvas");
    qmlRegisterType<Context2D>(uri, 1, 0, "Context2D");
    qmlRegisterUncreatableType<CanvasImage>(uri, 1, 0, "CanvasImage", QString());
    qmlRegisterUncreatableType<CanvasGradient>(uri, 1, 0, "Gradient", QString());
    qmlRegisterType<GPIOPinInput>(uri, 1, 0, "GPIOPinInput");
    qmlRegisterType<GPIOPinOutput>(uri, 1, 0, "GPIOPinOutput");
    qmlRegisterType<GPIOPinsInput>(uri, 1, 0, "GPIOPinsInput");
    qmlRegisterType<GPIOPinsOutput>(uri, 1, 0, "GPIOPinsOutput");
    qmlRegisterType<sqlite>(uri, 1, 0, "SqLite");
    qmlRegisterType<Upgrade>(uri, 1, 0, "Upgrade");
}

Q_EXPORT_PLUGIN2(systemplugin, SystemPlugin)
