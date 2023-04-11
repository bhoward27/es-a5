#ifndef _SHARED_DATA_STRUCT_H_
#define _SHARED_DATA_STRUCT_H_

#include <stdbool.h>
#include <stdint.h>

/*
WARNING:
Fields in the struct must be aligned to match ARM's alignment
    bool/char, uint8_t: byte aligned
    int/long, uint32_t: word (4 byte) aligned
    double, uint64_t: dword (8 byte) aligned
Add padding fields (char _p1) to pad out to alignment.
*/

#define NUM_PIXELS 8

// Explanation of struct member prefixes:
//  - Linux_ means that only the Linux application writes to this member.
//  - Pru_ means that only the PRU writes to this member.
typedef struct {
    uint32_t Linux_pixels[NUM_PIXELS];

    // Ideally this would be a bool, but setting to uint32 means I don't have to worry about padding.
    uint32_t Pru_joystickDown;
    uint32_t Pru_joystickRight;
} sharedMemStruct_t;

#endif
