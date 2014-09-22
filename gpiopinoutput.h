#ifndef GPIOPINOUTPUT_H
#define GPIOPINOUTPUT_H
#include <QDeclarativeItem>
#include <linux/types.h>

class GPIOPinOutput : public QDeclarativeItem
{
    Q_OBJECT
    Q_DISABLE_COPY(GPIOPinOutput)
    Q_PROPERTY(int pin READ pin WRITE setPin)


public:
    explicit GPIOPinOutput(QDeclarativeItem *parent = 0);
    ~GPIOPinOutput();
    int pin() const;
    void setPin(const int pin);
    Q_INVOKABLE int writeToPin(int val);
signals:
    
public slots:

private:
    int m_pin;

};

QML_DECLARE_TYPE(GPIOPinOutput)
#endif // GPIOPINOUTPUT_H
