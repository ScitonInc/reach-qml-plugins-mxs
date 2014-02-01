 TEMPLATE = lib
 CONFIG += qt plugin
 QT += declarative network 

 DESTDIR = lib
 OBJECTS_DIR = tmp
 MOC_DIR = tmp

 HEADERS += beeper.h \
            systemplugin.h \
            backlight.h \
            network.h

 SOURCES += beeper.cpp \
            systemplugin.cpp \
            backlight.cpp \
            network.cpp

