/// Module for reading the accelerometer repeatedly in a thread.

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "sharedDataStruct.h"

#define ACCELEROMETER_BUS_NUMBER 1
#define ACCELEROMETER_DEVICE_ADDRESS 0x1C
#define ACCELEROMETER_CTRL_REG_1_ADDRESS 0x2A
#define ACCELEROMETER_ACTIVE 1

#define ACCELEROMETER_STATUS_REGISTER_ADDRESS 0x00
#define ACCELEROMETER_OUT_X_MSB_ADDRESS 0x01
#define ACCELEROMETER_OUT_X_LSB_ADDRESS 0x02
#define ACCELEROMETER_OUT_Y_MSB_ADDRESS 0x03
#define ACCELEROMETER_OUT_Y_LSB_ADDRESS 0x04

#define ACCELEROMETER_MAX 16400

#define ACCELEROMETER_NUM_BYTES 7


void Accelerometer_init(volatile sharedMemStruct_t* pSharedDataArg);
void Accelerometer_waitForShutdown(void);

#endif