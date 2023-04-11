// Header file for the joystick module that exposes the direction enum and a couple of functions.

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "sharedDataStruct.h"

enum direction{none, down, right};

void Joystick_initializeJoystick(volatile sharedMemStruct_t* pSharedDataArg);
enum direction Joystick_checkWhichDirectionIsPressed(void);
void Joystick_cleanupJoystick(void);

#endif