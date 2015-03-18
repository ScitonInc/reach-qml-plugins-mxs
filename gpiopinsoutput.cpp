#include "gpiopinsoutput.h"
extern "C" {
#include "gpio.h"
}

GPIOPinsOutput::GPIOPinsOutput(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
}

int GPIOPinsOutput::writeToPins(int val){
    __s32 r;
    gpio_config_port(I2C_GPIO_ADDR, 0);
    r=gpio_rd_reg(I2C_GPIO_ADDR, REG_CONFIG);
    printf("Config Register = 0x%02x\n", r);

    gpio_wr_port(I2C_GPIO_ADDR, val);
    r=gpio_rd_reg(I2C_GPIO_ADDR, REG_OUTPUT);
    printf("Output Register = 0x%02x\n", r);
    return r;
}

int GPIOPinsOutput::readPins()
{
    __s32 r;
    gpio_config_port(I2C_GPIO_ADDR, 1);
    r=gpio_rd_reg(I2C_GPIO_ADDR, REG_CONFIG);

    r=gpio_rd_reg(I2C_GPIO_ADDR, REG_INPUT);
    return r;
}

GPIOPinsOutput::~GPIOPinsOutput()
{
}
