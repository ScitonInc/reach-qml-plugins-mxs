 TEMPLATE = lib
 CONFIG += qt plugin
 QT += declarative network 
 QT += script
 DESTDIR = lib
 OBJECTS_DIR = tmp
 MOC_DIR = tmp

 HEADERS += beeper.h \
            systemplugin.h \
            backlight.h \
            network.h \
            system.h \
            canvas.h \
            canvastimer.h \
            context2d.h

 SOURCES += beeper.cpp \
            systemplugin.cpp \
            backlight.cpp \
            network.cpp \
            system.cpp \
            canvas.cpp \
            canvastimer.cpp \
            context2d.cpp

