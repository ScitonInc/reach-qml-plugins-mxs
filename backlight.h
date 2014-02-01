#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <QObject>
#include <QFile>
#include <QTextStream>

#define BRIGHTNESS "/sys/devices/platform/mxs-bl.0/backlight/mxs-bl/brightness"
#define BL_POWER "/sys/devices/platform/mxs-bl.0/backlight/mxs-bl/bl_power"

class Backlight : public QObject
{
    Q_OBJECT

public:
    explicit Backlight(QObject *parent = 0);
    ~Backlight();

    Q_INVOKABLE int brightness(void);
    Q_INVOKABLE void setBrightness(int duration);
    Q_INVOKABLE void enableBacklight(void);
    Q_INVOKABLE void disableBacklight(void);

private:
    QFile m_brightness_file;
    QFile m_backlight_file;
};

#endif
