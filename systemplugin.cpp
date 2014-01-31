#include "systemplugin.h"
#include "beeper.h"

#include <qdeclarative.h>

void SystemPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<Beeper>(uri, 1, 0, "Beeper");
}

Q_EXPORT_PLUGIN2(systemplugin, SystemPlugin);
