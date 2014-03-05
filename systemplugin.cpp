#include "systemplugin.h"
#include "beeper.h"
#include "backlight.h"
#include "network.h"
#include "system.h"
#include "context2d.h"
#include "canvas.h"
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
}

Q_EXPORT_PLUGIN2(systemplugin, SystemPlugin);
