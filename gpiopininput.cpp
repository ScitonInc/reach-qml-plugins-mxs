#include "gpiopininput.h"
extern "C" {
#include "gpio.h"
}
GPIOPinInput::GPIOPinInput(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    gpio_config_pin(I2C_GPIO_ADDR, m_pin, 1);
    setState(gpio_rd_reg_bit(I2C_GPIO_ADDR, REG_INPUT, m_pin));

    if (m_pollPin)
    {
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(readPin()));
        timer->start(10);
    }
}

int GPIOPinInput::pin() const
{
    return m_pin;
}

void GPIOPinInput::setPin(int pin)
{
    m_pin = pin;
}

bool GPIOPinInput::pollPin() const
{
    return m_pollPin;
}

void GPIOPinInput::setPollPin(bool pollPin)
{
    m_pollPin = pollPin;
}

int GPIOPinInput::state() const
{
    return m_state;
}

void GPIOPinInput::setState(int state)
{
    m_state = state;
    emit stateChanged();
}

void GPIOPinInput::readPin()
{
    //Set pin as an output
    __s32 r;
    gpio_config_pin(I2C_GPIO_ADDR, m_pin, 1);
    r=gpio_rd_reg(I2C_GPIO_ADDR, REG_CONFIG);

    r = gpio_rd_reg_bit(I2C_GPIO_ADDR, REG_INPUT, m_pin);
    if (m_state != r)
        setState(r);
}

GPIOPinInput::~GPIOPinInput()
{
}
