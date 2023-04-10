// This module tracks in a thread-safe manner whether shutdown has been requested, and allows for threads to
// request shutdown. If shutdown has been requested, threads should be polite and stop!
#ifndef SHUTDOWN_H_
#define SHUTDOWN_H_

#include <stdatomic.h>
#include <stdbool.h>

bool isShutdownRequested(void);
void requestShutdown(void);

#endif
