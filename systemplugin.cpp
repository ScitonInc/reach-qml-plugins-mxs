#include "systemplugin.h"
#include "beeper.h"
#include "backlight.h"
#include "network.h"
#include "system.h"

#include <QtDeclarative>

void SystemPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<Beeper>(uri, 1, 0, "Beeper");
    qmlRegisterType<Backlight>(uri, 1, 0, "Backlight");
    qmlRegisterType<Network>(uri, 1, 0, "Network");
    qmlRegisterType<System>(uri, 1, 0, "System");
}

Q_EXPORT_PLUGIN2(systemplugin, SystemPlugin);
