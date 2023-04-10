#include "shutdown.h"

static atomic_int requestedShutdown = 0; // Initialize to "not requested".

bool isShutdownRequested(void)
{
    return requestedShutdown;
}

void requestShutdown(void)
{
    requestedShutdown = 1;
}
