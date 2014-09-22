 TEMPLATE = lib
 CONFIG += qt plugin
 QT += declarative network sql
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
            context2d.h \
    i2c-dev.h \
    gpiopinsoutput.h \
    gpiopinsinput.h \
    gpiopinoutput.h \
    gpiopininput.h \
    gpio.h \
    sqlite.h \
    worker.h \
    upgrade.h

 SOURCES += beeper.cpp \
            systemplugin.cpp \
            backlight.cpp \
            network.cpp \
            system.cpp \
            canvas.cpp \
            canvastimer.cpp \
            context2d.cpp \
    gpiopinsoutput.cpp \
    gpiopinsinput.cpp \
    gpiopinoutput.cpp \
    gpiopininput.cpp \
    sqlite.cpp \
    worker.cpp \
    upgrade.cpp

