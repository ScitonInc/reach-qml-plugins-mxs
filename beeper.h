#ifndef BEEPER_H
#define BEEPER_H

#include <qobject.h>
#include <qfile.h>
#include <qtextstream.h>

#define BEEPER "/sys/kernel/beeper/beep"
#define VOLUME "/sys/kernel/beeper/vol"
#define FREQUENCY "/sys/kernel/beeper/freq"
#define DURATION "/sys/kernel/beeper/duration"

class Beeper : public QObject
{
    Q_OBJECT

public:
    explicit Beeper(QObject *parent = 0);
    ~Beeper();

    Q_INVOKABLE void beep(void);
    Q_INVOKABLE int volume(void);
    Q_INVOKABLE void setVolume(int volume);
    Q_INVOKABLE int  frequency(void);
    Q_INVOKABLE void setFrequency(int freq);
    Q_INVOKABLE int  duration(void);
    Q_INVOKABLE void setDuration(int duration);

private:
    QFile m_beeper_file;
    QFile m_volume_file;
    QFile m_frequency_file;
    QFile m_duration_file;
    QTextStream m_beeper_out;
};

#endif
