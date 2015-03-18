#include "gpiopinoutput.h"
extern "C" {
#include "gpio.h"
}

GPIOPinOutput::GPIOPinOutput(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
}

int GPIOPinOutput::pin() const
{
    return m_pin;
}

 void GPIOPinOutput::setPin(int pin)
 {
     m_pin = pin;
 }

int GPIOPinOutput::writeToPin(int val){
    __s32 r;
    int rval = gpio_config_pin(I2C_GPIO_ADDR, m_pin, 0);
    r=gpio_rd_reg(I2C_GPIO_ADDR, REG_CONFIG);
    printf("Config Register = 0x%02x\n", r);

    int test = gpio_wr_pin(I2C_GPIO_ADDR, m_pin, val);
    r=gpio_rd_reg(I2C_GPIO_ADDR, REG_OUTPUT);
    printf("Output Register = 0x%02x\n", r);
    return r;
}

int GPIOPinOutput::readPin(){
    __s32 r;
    int rval = gpio_config_pin(I2C_GPIO_ADDR, m_pin, 1);
    r=gpio_rd_reg(I2C_GPIO_ADDR, REG_CONFIG);
    r = gpio_rd_reg_bit(I2C_GPIO_ADDR, REG_INPUT, m_pin);
    return r;
}

GPIOPinOutput::~GPIOPinOutput()
{
}
