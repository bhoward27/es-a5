// Implements various convenience functions.

#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>

#include "int_typedefs.h"
#include "return_val.h"

#define SMALL_STRING_LEN 128
#define MEDIUM_STRING_LEN 1024

#define NUM_MS_PER_S 1000

#define NUM_ELEMS(array) sizeof((array)) / sizeof((array)[0])

int overwriteFile(const char* filePath, const char* string, bool exitOnFailure);
int readFile(char* filePath, void* outData, size_t numBytesPerItem, size_t numItems, bool exitOnFailure);
int runCommand(const char* command);
int64 getTimeInMs(void);
void sleepForMs(int64 delayInMs);
void busyWaitForMs(int64 delayInMs);
int64 int64_min(int64 a, int64 b);

#endif