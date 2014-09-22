#include "gpiopinsinput.h"
extern "C" {
#include "gpio.h"
}

GPIOPinsInput::GPIOPinsInput(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    m_state = -1;
    gpio_config_port(I2C_GPIO_ADDR, 1);
    __s32 r=gpio_rd_reg(I2C_GPIO_ADDR, REG_CONFIG);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readRegister()));
    timer->start(10);
}

int GPIOPinsInput::state() const
{
    return m_state;
}

QString GPIOPinsInput::getHexState()
{
    return QString::number(m_state, 16);
}

void GPIOPinsInput::setState(int state)
{
    m_state = state;
    emit stateChanged();
}

void GPIOPinsInput::readRegister()
{
    __s32 r;
    r=gpio_rd_reg(I2C_GPIO_ADDR, REG_INPUT);
    if (m_state != r)
        setState(r);
}

GPIOPinsInput::~GPIOPinsInput()
{
}
