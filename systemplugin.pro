 TEMPLATE = lib
 CONFIG += qt plugin
 QT += declarative

 DESTDIR = lib
 OBJECTS_DIR = tmp
 MOC_DIR = tmp

 HEADERS += beeper.h \
            systemplugin.h

 SOURCES += beeper.cpp \
            systemplugin.cpp

