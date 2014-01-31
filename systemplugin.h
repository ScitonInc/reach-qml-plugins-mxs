 #ifndef SYSTEMPLUGIN_H
 #define SYSTEMPLUGIN_H

 #include <qdeclarativeextensionplugin.h>

 class SystemPlugin : public QDeclarativeExtensionPlugin
 {
     Q_OBJECT
 public:
     void registerTypes(const char *uri);
 };

 #endif
