#ifndef GPIO_H
#define GPIO_H

#include <QDeclarativeItem>
#include <i2c-dev.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

/* define the i2c slave address */
#define I2C_GPIO_ADDR 0x3E

#define REG_INPUT   0x00
#define REG_OUTPUT  0x01
#define REG_INVERT  0x02
#define REG_CONFIG  0x03

static int gpio_i2c_reg_wr(int fd, __u8 reg, __u8 val)
{
    /* write to the specified register of i2c port expander */
    if(i2c_smbus_write_byte_data(fd, reg, val) < 0)
        fprintf( stderr, "failed to write to slave %d\n",errno );
        return 2;

    return 0;
}

static __s32 gpio_i2c_reg_rd(int fd, __u8 reg)
{
    __s32 reg_out;

    //Read register
    reg_out = i2c_smbus_read_byte_data(fd, reg);
    if(reg_out < 0) {
        fprintf( stderr, "failed to read from slave \n" );
        return -1;
    }

    return reg_out;
}


static int gpio_config_port(int addr, __u8 v)
{
    int fd;

    /* open the i2c adapter */
    if((fd = open( "/dev/i2c-1", O_RDWR )) == -1) {
        fprintf( stderr, "Failed to open /dev/i2c-1\n" );
        return 2;
    }

    /* set the slave device address we want to talk to */
    if( ioctl( fd, I2C_SLAVE, addr ) < 0 ) {
        fprintf( stderr, "Failed to set slave address: %d\n", addr);
        close(fd);
        return 2;
    }

    if(0!=gpio_i2c_reg_wr(fd, REG_CONFIG, v)) {
        close(fd);
        return 2;
    }

    close(fd);
    return 0;
}

static int gpio_config_pin(int addr, __u8 pin, __u8 dir)
{
    int fd;
    __s32 reg_out = -1;

    // Check for valid pin or value.
    if(pin > 7) return 2; //error pin out of range.
    if(dir > 1) return 2;  //error value out of range.

    /* open the i2c adapter */
    if((fd = open( "/dev/i2c-1", O_RDWR )) == -1) {
        fprintf( stderr, "Failed to open /dev/i2c-1\n" );
        return 2;
    }

    /* set the slave device address we want to talk to */
    if( ioctl( fd, I2C_SLAVE, addr ) < 0 ) {
        fprintf( stderr, "Failed to set slave address: %d\n", addr);
        close(fd);
        return 2;
    }

    //Read config register
    reg_out = gpio_i2c_reg_rd(fd, REG_CONFIG);
    if(reg_out < 0) {
        close(fd);
        return 2;
    }

    //set or clear the pin based on v.
    reg_out = (0!=dir)?(reg_out |= (dir<<pin)):(reg_out &= ~(0x01<<pin));

    if(0!=gpio_i2c_reg_wr(fd, REG_CONFIG, (__u8)reg_out)) {
        close(fd);
        return 2;
    }

    /* clean up */
    close(fd);

    return 0;
}

static int gpio_wr_port(int addr, __u8 v)
{
    int fd;

    /* open the i2c adapter */
    if((fd = open( "/dev/i2c-1", O_RDWR )) == -1) {
        fprintf( stderr, "Failed to open /dev/i2c-1\n" );
        return 2;
    }

    /* set the slave device address we want to talk to */
    if( ioctl( fd, I2C_SLAVE, addr ) < 0 ) {
        fprintf( stderr, "Failed to set slave address: %d\n", addr);
        close(fd);
        return 2;
    }

    if(0!=gpio_i2c_reg_wr(fd, REG_OUTPUT, v)) {
        close(fd);
        return 2;
    }

    /* clean up */
    close(fd);

    return 0;

}

static int gpio_wr_pin(int addr, __u8 pin, __u8 v)
{
    int fd;
    __s32 reg_out;

    // Check for valid pin or value.
    if(pin > 7) return 2; //error pin out of range.
    if(v > 1) return 2;  //error value out of range.

    /* open the i2c adapter */
    if((fd = open( "/dev/i2c-1", O_RDWR )) == -1) {
        fprintf( stderr, "Failed to open /dev/i2c-1\n" );
        return 2;
    }

    /* set the slave device address we want to talk to */
    if( ioctl( fd, I2C_SLAVE, addr ) < 0 ) {
        fprintf( stderr, "Failed to set slave address: %d\n", addr);
        close(fd);
        return 2;
    }

    //Read output register.
    reg_out = gpio_i2c_reg_rd(fd, REG_OUTPUT);
    if(reg_out < 0) {
        fprintf( stderr, "Failed to read output register\n");
        close(fd);
        return 2;
    }

    //set or clear the pin based on v.
    reg_out = (0!=v)?(reg_out |= (v<<pin)):(reg_out &= ~(0x01<<pin));

    if(0!=gpio_i2c_reg_wr(fd, REG_OUTPUT, (__u8)reg_out)) {
        close(fd);
        return 2;
    }

    /* clean up */
    close(fd);

    return 0;
}

static __s32 gpio_rd_reg(int addr, __u8 reg)
{
    int fd;
    __s32 ret;

    /* open the i2c adapter */
    if((fd = open( "/dev/i2c-1", O_RDWR )) == -1) {
        fprintf( stderr, "Failed to open /dev/i2c-1\n" );
        return -1;
    }

    /* set the slave device address we want to talk to */
    if( ioctl( fd, I2C_SLAVE, addr ) < 0 ) {
        fprintf( stderr, "Failed to set slave address: %d\n", addr);
        close(fd);
        return -1;
    }

    ret = gpio_i2c_reg_rd(fd, reg);
    if(ret<0) {
        close(fd);
        return -1;
    }

    /* clean up */
    close(fd);

    return ret;

}

static __s32 gpio_rd_reg_bit(int addr, __u8 reg, __u8 pin)
{
    int fd;
    __s32 ret;

    /* open the i2c adapter */
    if((fd = open( "/dev/i2c-1", O_RDWR )) == -1) {
        fprintf( stderr, "Failed to open /dev/i2c-1\n" );
        return -1;
    }

    /* set the slave device address we want to talk to */
    if( ioctl( fd, I2C_SLAVE, addr ) < 0 ) {
        fprintf( stderr, "Failed to set slave address: %d\n", addr);
        close(fd);
        return -1;
    }

    ret = gpio_i2c_reg_rd(fd, reg);
    if(ret<0) {
        close(fd);
        return -1;
    }

    ret = (ret>>pin)&0x01;

    /* clean up */
    close(fd);

    return ret;

}

#endif // GPIO_H

