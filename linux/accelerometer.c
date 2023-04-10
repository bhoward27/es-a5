#include <pthread.h>
#include <stdio.h>

#include "accelerometer.h"
#include "shutdown.h"
#include "utils.h"
#include "log.h"
#include "i2c.h"
#include "colours.h"

static pthread_t thread;
static int i2cFd;
static float x;
static float y;
static volatile sharedMemStruct_t* pSharedPru0 = NULL;


// TODO: Probably move these elsewhere.
static float targetX;
static float targetY;

static void setAllPixels(volatile uint32_t pixels[], uint32_t colour) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pixels[i] = colour;
    }
}

static void processX(float x) {
    // X axis changes the colour.
    //      Red means need to rotate to the left. Green means rotate to the right. Blue means don't move.

    float minOnTarget = targetX - ACCELEROMETER_TARGET_DELTA;
    float maxOnTarget = targetX + ACCELEROMETER_TARGET_DELTA;
    if (minOnTarget <= x && x <= maxOnTarget) {
        setAllPixels(pSharedPru0->Linux_pixels, BLUE);
    }
    else if (x < minOnTarget) {
        setAllPixels(pSharedPru0->Linux_pixels, GREEN);
    }
    else if (x > maxOnTarget) {
        setAllPixels(pSharedPru0->Linux_pixels, RED);
    }
}

static void* Accelerometer_run(void* args);

void Accelerometer_init(volatile sharedMemStruct_t* pSharedDataArg) {
    pSharedPru0 = pSharedDataArg;
    i2cFd = I2c_openBus(ACCELEROMETER_BUS_NUMBER, ACCELEROMETER_DEVICE_ADDRESS);

    // Set CTRL_REG_1 to active.
    I2c_write(i2cFd, ACCELEROMETER_CTRL_REG_1_ADDRESS, ACCELEROMETER_ACTIVE);

    // TODO: Remove this.
    targetX = 0.2;
    targetY = -0.3;

    int res = pthread_create(&thread, NULL, Accelerometer_run, NULL);
    if (res != 0) {
        SYS_DIE("pthread_create failed.\n");
    }
}

static void* Accelerometer_run(void* args) {
    while (!isShutdownRequested()) {
        uint8 buffer[ACCELEROMETER_NUM_BYTES];
        I2c_read(i2cFd, ACCELEROMETER_STATUS_REGISTER_ADDRESS, buffer, ACCELEROMETER_NUM_BYTES);

        int16 rawX = (buffer[ACCELEROMETER_OUT_X_MSB_ADDRESS] << 8) | (buffer[ACCELEROMETER_OUT_X_LSB_ADDRESS]);
        int16 rawY = (buffer[ACCELEROMETER_OUT_Y_MSB_ADDRESS] << 8) | (buffer[ACCELEROMETER_OUT_Y_LSB_ADDRESS]);

        x = (float) rawX / ACCELEROMETER_MAX;
        y = (float) rawY / ACCELEROMETER_MAX;


        // LOG(LOG_LEVEL_DEBUG, "Raw x = %d\n", rawX);
        LOG(LOG_LEVEL_DEBUG, "x = %f\n", x);
        // LOG(LOG_LEVEL_DEBUG, "Raw y = %d\n", rawY);
        LOG(LOG_LEVEL_DEBUG, "y = %f\n", y);

        // X axis changes the colour.
        //      Red means need to rotate to the left. Green means rotate to the right. Blue means don't move.
        // Y axis changes which pixels are illuminated.
        //      Position of the pixel indicates where BBG is pointing relative to the target. So if an upper pixel is
        //      on that means that BBG is pointing too far up, for example.
        //      All pixels illuminate if Y axis is pointing to the target.

        processX(x);

        // TODO: Decrease the sleep time.
        sleepForMs(250);
    }

    return NULL;
}

float Accelerometer_getX(void) {
    return x;
}

float Accelerometer_getY(void) {
    return y;
}

void Accelerometer_waitForShutdown(void) {
    int res = pthread_join(thread, NULL);
    if (res != 0) {
        SYS_WARN("pthread_join failed.\n");
    }
    else {
        I2c_closeBus(i2cFd);
    }
}