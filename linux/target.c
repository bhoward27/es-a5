#include <stdlib.h>
#include <time.h>

#include "target.h"
#include "log.h"

static float targetX = 0.0;
static float targetY = 0.0;

static float Target_rng(void)
{
    static bool isFirstCall = true;
    if (isFirstCall) {
        // Seed RNG.
        srand(time(0));
        isFirstCall = false;
    }

    // Shift range from [0, RAND_MAX] to [-RAND_MAX/2, RAND_MAX/2].
    double r = rand() - RAND_MAX / 2;

    // Shrink range from [-RAND_MAX/2, RAND_MAX/2] to [-0.5, 0.5].
    float s = r / RAND_MAX;
    return s;
}

void Target_setRandom(void)
{
    targetX = Target_rng();
    targetY = Target_rng();
    LOG(LOG_LEVEL_NOTICE, "New target = (%f, %f)\n", targetX, targetY);
}


bool Target_isHit(float x, float y, float targetX, float targetY)
{
    return (Target_isOnTarget(x, targetX) && Target_isOnTarget(y, targetY));
}

bool Target_isOnTarget(float a, float targetA)
{
    float minOnTarget = targetA - TARGET_DELTA;
    float maxOnTarget = targetA + TARGET_DELTA;
    return (minOnTarget <= a && a <= maxOnTarget);
}

bool Target_fire(float x, float y)
{
    if (Target_isHit(x, y, targetX, targetY)) {
        LOG(LOG_LEVEL_NOTICE, "Hit!\n");
        Target_setRandom();
        return true;
    }

    LOG(LOG_LEVEL_NOTICE, "Miss.\n");
    return false;
}

float Target_getX(void)
{
    return targetX;
}

float Target_getY(void)
{
    return targetY;
}