#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "accelerometer.h"
#include "shutdown.h"
#include "utils.h"
#include "log.h"
#include "i2c.h"
#include "colours.h"
#include "target.h"

static pthread_t thread;
static int i2cFd;
static float x;
static float y;
static volatile sharedMemStruct_t* pSharedPru0 = NULL;

// Local target coordinates. Need to be updated with Target_getX() and Target_getY().
static float targetX = 0.0;
static float targetY = 0.0;

static void* Accelerometer_run(void* args);

static void setAllPixels(volatile uint32_t pixels[], uint32_t colour) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pixels[i] = colour;
    }
}

static uint32 processX(float x) {
    // X axis changes the colour.
    //      Red means need to rotate to the left. Green means rotate to the right. Blue means don't move.

    if (Target_isOnTarget(x, targetX)) {
        // setAllPixels(pSharedPru0->Linux_pixels, BLUE);
        return BLUE;
    }
    else if (x < targetX) {
        return GREEN;
    }
    else if (x > targetX) {
        return RED;
    }
    else { // Should be impossible
        return OFF;
    }
}

static int32 dim(uint32 colour) {
    switch (colour) {
        case GREEN:
            return DIM_GREEN;
        case RED:
            return DIM_RED;
        case BLUE:
            return DIM_BLUE;
        default:
            LOG(LOG_LEVEL_WARN, "Dim colour %x not implemented.\n", colour);
            return OFF;
    }
}

static void processY(float y, uint32 colour) {
    // Y axis changes which pixels are illuminated.
    //      Position of the pixel indicates where BBG is pointing relative to the target. So if an upper pixel is
    //      on that means that BBG is pointing too far up, for example.
    //      All pixels illuminate if Y axis is pointing to the target.
    if (Target_isOnTarget(y, targetY)) {
        setAllPixels(pSharedPru0->Linux_pixels, colour);
    }
    else {
        int centreIndex = 0; // Between 0 and (NUM_PIXELS - 1).
        float a = (NUM_PIXELS / 2 - 1) * 1.8; // The last number there is arbitrary -- determined "by feel".
        if (y < targetY) {
            centreIndex = round((NUM_PIXELS / 2 - 1) - ((targetY - y) * a));
        }
        else if (y > targetY) {
            centreIndex = round((y - targetY) * a + NUM_PIXELS / 2);
        }
        int topIndex = centreIndex + 1;
        int bottomIndex = centreIndex - 1;
        if (centreIndex > NUM_PIXELS - 1) {
            centreIndex = NUM_PIXELS - 1;
            bottomIndex = -1; // Don't show bottom pixel.
        }
        else if (centreIndex < 0) {
            centreIndex = 0;
            topIndex = NUM_PIXELS; // Don't show top pixel.
        }

        // Turn all pixels off.
        memset((void*) pSharedPru0->Linux_pixels, 0, sizeof(pSharedPru0->Linux_pixels));

        // Turn desired pixels on.
        if (topIndex < NUM_PIXELS) {
            pSharedPru0->Linux_pixels[topIndex] = dim(colour);
        }
        pSharedPru0->Linux_pixels[centreIndex] = colour;
        if (bottomIndex >= 0) {
            pSharedPru0->Linux_pixels[bottomIndex] = dim(colour);
        }
    }
}

void Accelerometer_init(volatile sharedMemStruct_t* pSharedDataArg) {
    pSharedPru0 = pSharedDataArg;
    i2cFd = I2c_openBus(ACCELEROMETER_BUS_NUMBER, ACCELEROMETER_DEVICE_ADDRESS);

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

        int16 rawX = (buffer[ACCELEROMETER_OUT_X_MSB_ADDRESS] << 8) | (buffer[ACCELEROMETER_OUT_X_LSB_ADDRESS]);
        int16 rawY = (buffer[ACCELEROMETER_OUT_Y_MSB_ADDRESS] << 8) | (buffer[ACCELEROMETER_OUT_Y_LSB_ADDRESS]);

        x = (float) rawX / ACCELEROMETER_MAX;
        y = (float) rawY / ACCELEROMETER_MAX;

        // Print current coordinates of BBG orientation.
        // LOG(LOG_LEVEL_DEBUG, "x = %f\n", x);
        // LOG(LOG_LEVEL_DEBUG, "y = %f\n", y);

        // Update local target coordinates.
        targetX = Target_getX();
        targetY = Target_getY();

        // Set NeoPixel colours based on BBG orientation's proximity to target.
        uint32 colour = processX(x);
        processY(y, colour);

        sleepForMs(50);
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