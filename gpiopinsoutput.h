#ifndef GPIOPINSOUTPUT_H
#define GPIOPINSOUTPUT_H

#include <QDeclarativeItem>
#include <linux/types.h>
#include <QDebug>

class GPIOPinsOutput : public QDeclarativeItem
{
    Q_OBJECT
    Q_DISABLE_COPY(GPIOPinsOutput)
public:
    explicit GPIOPinsOutput(QDeclarativeItem *parent = 0);
    ~GPIOPinsOutput();
    Q_INVOKABLE int writeToPins(int val);
    Q_INVOKABLE int readPins();

    
signals:
    
public slots:
    
};

QML_DECLARE_TYPE(GPIOPinsOutput)

#endif // GPIOPINSOUTPUT_H
