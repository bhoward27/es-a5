/**
 * Provide functions related to the randomly generated target coordinates, such as:
 *      - Generating new targets
 *      - Firing at the target
 *      - Determining if a coordinate is on target
 */
#ifndef TARGET_H_
#define TARGET_H_

#include <stdbool.h>
#include "int_typedefs.h"

#define TARGET_DELTA 0.1

void Target_setRandom(void);
bool Target_isHit(float x, float y, float targetX, float targetY);
bool Target_isOnTarget(float a, float targetA);
bool Target_fire(float x, float y);
float Target_getX(void);
float Target_getY(void);
uint8 Target_getHits(void);

#endif