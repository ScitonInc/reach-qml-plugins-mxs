#include "beeper.h"

#include <QDebug>

Beeper::Beeper(QObject *parent) : QObject(parent)
    ,m_beeper_file(BEEPER)
    ,m_volume_file(VOLUME)
    ,m_frequency_file(FREQUENCY)
    ,m_duration_file(DURATION)
    ,m_beeper_out()
{
    m_beeper_file.open(QIODevice::ReadWrite);
    m_beeper_out.setDevice(&m_beeper_file);
}

Beeper::~Beeper()
{
    if(m_beeper_file.isOpen()) {
        m_beeper_file.close();
    }

    if(m_volume_file.isOpen()) {
        m_volume_file.close();
    }

    if(m_frequency_file.isOpen()) {
        m_frequency_file.close();
    }

    if(m_duration_file.isOpen()) {
        m_duration_file.close();
    }
}

void Beeper::beep()
{
    m_beeper_out << QString::number(1).toLatin1() << endl;
}

int Beeper::volume()
{
    m_volume_file.open(QIODevice::ReadOnly);
    QTextStream in(&m_volume_file);
    QString s = in.readLine();
    m_volume_file.close();

    return s.toInt();
}

void Beeper::setVolume(int volume)
{
    m_volume_file.open(QIODevice::ReadWrite);
    QTextStream out(&m_volume_file);

    out << QString::number(volume).toLatin1() << endl;
    m_volume_file.close();
}

void Beeper::setFrequency(int freq)
{
    m_frequency_file.open(QIODevice::ReadWrite);
    QTextStream out(&m_frequency_file);

    out << QString::number(freq).toLatin1() << endl;
    m_frequency_file.close();
}

int Beeper::frequency()
{
    m_frequency_file.open(QIODevice::ReadOnly);
    QTextStream in(&m_frequency_file);
    QString s = in.readLine();
    m_frequency_file.close();

    return s.toInt();
}

void Beeper::setDuration(int duration)
{
    m_duration_file.open(QIODevice::ReadWrite);
    QTextStream out(&m_duration_file);
    out << QString::number(duration).toLatin1() << endl;
    m_duration_file.close();
}

int Beeper::duration()
{
    m_duration_file.open(QIODevice::ReadOnly);
    QTextStream in(&m_duration_file);
    QString s = in.readLine();
    m_duration_file.close();

    return s.toInt();
}
