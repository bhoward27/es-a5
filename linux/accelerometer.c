#include <pthread.h>
#include <stdio.h>

#include "accelerometer.h"
#include "shutdown.h"
#include "utils.h"
#include "log.h"
#include "i2c.h"

static pthread_t thread;
static int i2cFd;
static int16 x;
static int16 y;

static void* Accelerometer_run(void* args);

void Accelerometer_init(void) {
    i2cFd = I2c_openBus(1, 0x1C);

    // Set CTRL_REG_1 to active.
    I2c_write(i2cFd, ACCELEROMETER_CTRL_REG_1_ADDRESS, ACCELEROMETER_ACTIVE);

    int res = pthread_create(&thread, NULL, Accelerometer_run, NULL);
    if (res != 0) {
        SYS_DIE("pthread_create failed.\n");
    }
}

static void* Accelerometer_run(void* args) {
    while (!isShutdownRequested()) {
        uint8 buffer[ACCELEROMETER_NUM_BYTES];
        I2c_read(i2cFd, ACCELEROMETER_STATUS_REGISTER_ADDRESS, buffer, ACCELEROMETER_NUM_BYTES);

        x = (buffer[ACCELEROMETER_OUT_X_MSB_ADDRESS] << 8) | (buffer[ACCELEROMETER_OUT_X_LSB_ADDRESS]);
        y = (buffer[ACCELEROMETER_OUT_Y_MSB_ADDRESS] << 8) | (buffer[ACCELEROMETER_OUT_Y_LSB_ADDRESS]);

        printf("Acceleration x = %d\n", x);
        printf("Acceleration y = %d\n", y);

        sleepForMs(250);
    }

    return NULL;
}

int16 Accelerometer_getX(void) {
    return x;
}

int16 Accelerometer_getY(void) {
    return y;
}

void Accelerometer_waitForShutdown(void) {
    int res = pthread_join(thread, NULL);
    if (res != 0) {
        SYS_WARN("pthread_join failed.\n");
    }
}