// Provides functions to enable and write to an I2C bus.
#ifndef I2C_H_
#define I2C_H_

#include "gpio.h"

// We assume each I2C bus has only two pins: data and clock.
#define I2C_BUS_NUM_PINS 2

#define I2C_BUS_1_GPIO_HEADER "p9"
#define I2C_BUS_1_GPIO_DATA_PIN "18"
#define I2C_BUS_1_GPIO_CLOCK_PIN "17"

#define I2C_BUS_1_GPIO_EXTENDER_ADDRESS 0x20

#define I2C_DEV_FILE_PATH_PREFIX "/dev/i2c-"

extern const GpioInfo I2c_bus1GpioPinInfo[I2C_BUS_NUM_PINS];

int I2c_enable(const GpioInfo busGpioInfo[], uint8 busNumber, uint8 gpioExtenderAddress);
int I2c_write(int i2cFd, uint8 registerAddress, uint8 value);
int I2c_openBus(uint8 busNumber, uint8 deviceAddress);
int I2c_closeBus(int busFd);
void I2c_read(int i2cFd, uint8 registerAddress, uint8 outBuffer[], int numBytes);

#endif