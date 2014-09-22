#ifndef GPIOPININPUT_H
#define GPIOPININPUT_H

#include <QDeclarativeItem>
#include <QTimer>
#include <linux/types.h>

class GPIOPinInput : public QDeclarativeItem
{
    Q_OBJECT
    Q_DISABLE_COPY(GPIOPinInput)
    Q_PROPERTY(int pin READ pin WRITE setPin)
    Q_PROPERTY(bool pollPin READ pollPin WRITE setPollPin)
    Q_PROPERTY(int state READ state WRITE setState NOTIFY stateChanged)
public:
    explicit GPIOPinInput(QDeclarativeItem *parent = 0);
    ~GPIOPinInput();
    int pin() const;
    void setPin(const int pin);
    bool pollPin() const;
    void setPollPin(const bool pollPin);
    int state() const;
    void setState(const int state);
signals:
    void stateChanged();
public slots:
    void readPin();
private:
    int m_pin;
    bool m_pollPin = true;
    __s32 m_state;

};

QML_DECLARE_TYPE(GPIOPinInput)
#endif // GPIOPININPUT_H
