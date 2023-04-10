#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>

#include "i2c.h"
#include "utils.h"
#include "log.h"
#include "int_typedefs.h"

const GpioInfo I2c_bus1GpioPinInfo[I2C_BUS_NUM_PINS] = {
    {I2C_BUS_1_GPIO_HEADER, I2C_BUS_1_GPIO_DATA_PIN},
    {I2C_BUS_1_GPIO_HEADER, I2C_BUS_1_GPIO_CLOCK_PIN}
};

int I2c_enable(const GpioInfo busGpioInfo[], uint8 busNumber, uint8 gpioExtenderAddress)
{
    int numSuccessful = 0;
    for (int i = 0; i < I2C_BUS_NUM_PINS; i++) {
        int res = Gpio_precheckSetPinMode(busGpioInfo[i].header, busGpioInfo[i].pin, "i2c", GPIO_MAX_MODE_LEN);
        if (res == COMMAND_SUCCESS) numSuccessful++;
    }
    if (numSuccessful != I2C_BUS_NUM_PINS) {
        LOG(LOG_LEVEL_WARN, "%s(%p, %u, %u) FAILED.\n", __func__, busGpioInfo, busNumber, gpioExtenderAddress);
    }
    if (numSuccessful != I2C_BUS_NUM_PINS) return -1;

    int i2cFd = I2c_openBus(busNumber, gpioExtenderAddress);

    // Enable output on all pins for GPIO extender.
    numSuccessful = 0;
    for (uint8 i = 0; i < 2; i++) {
        // Write zeroes to register address 0x00 and 0x01. (NOTE: Zed Cape Red would need different addresses).
        int res = I2c_write(i2cFd, i, 0x00);
        if (res == OK) numSuccessful++;
    }
    if (numSuccessful == I2C_BUS_NUM_PINS) {
        LOG(LOG_LEVEL_DEBUG, "%s(%p, %u, %u) SUCCEEDED.\n", __func__, busGpioInfo, busNumber, gpioExtenderAddress);
    }
    else {
        LOG(LOG_LEVEL_WARN, "%s(%p, %u, %u) FAILED.\n", __func__, busGpioInfo, busNumber, gpioExtenderAddress);
        return -1;
    }

    return i2cFd;
}

int I2c_openBus(uint8 busNumber, uint8 deviceAddress)
{
    char busFilePath[MEDIUM_STRING_LEN];
    snprintf(busFilePath, MEDIUM_STRING_LEN, "%s%u", I2C_DEV_FILE_PATH_PREFIX, busNumber);
    int i2cFileDesc = open(busFilePath, O_RDWR);
    if (i2cFileDesc == -1) {
        SYS_DIE("Failed to open '%s'.\n", busFilePath);
    }

    int result = ioctl(i2cFileDesc, I2C_SLAVE, deviceAddress);
    if (result < 0) {
        SYS_DIE("Failed to set I2C device 0x%x to slave address.\n", deviceAddress);
    }

    return i2cFileDesc;
}

int I2c_closeBus(int busFd)
{
    int res = close(busFd);
    if (res == -1) {
        SYS_WARN("Failed to close I2C bus with file descriptor = %d.\n", busFd);
        return ERR_CLOSE;
    }
    return OK;
}

int I2c_write(int i2cFd, uint8 registerAddress, uint8 value)
{
    uint8 buffer[2];
    buffer[0] = registerAddress;
    buffer[1] = value;
    int res = write(i2cFd, buffer, sizeof(buffer));
    if (res != sizeof(buffer)) {
        SYS_WARN("%s(%d, %u, %u) failed.\n", __func__, i2cFd, registerAddress, value);
        return ERR_WRITE;
    }
    return OK;
}

void I2c_read(int i2cFd, uint8 registerAddress, uint8 outBuffer[], int numBytes)
{
    // To read a register, must first write the address
    int res = write(i2cFd, &registerAddress, sizeof(registerAddress));
    if (res != sizeof(registerAddress)) {
        perror("I2C: Unable to write to i2c register.");
        exit(1);
    }
    // Now read the value and return it
    res = read(i2cFd, outBuffer, numBytes);
    if (res != numBytes) {
        perror("I2C: Unable to read from i2c register");
        exit(1);
    }
}
