#include <stdint.h>
#include <stdbool.h>
#include <pru_cfg.h>
#include <string.h>
#include "resource_table_empty.h"
#include "../linux/sharedDataStruct.h"

/*
Reference for shared RAM:
https://markayoder.github.io/PRUCookbook/05blocks/blocks.html#_controlling_the_pwm_frequency
*/

// GPIO Configuration
// ------------------
volatile register uint32_t __R30; // output GPIO register.
volatile register uint32_t __R31; // input GPIO register.

// GPIO Output: p8.12 = pru0_pru_r30_14
//  = LEDDP2 (Turn on/off right 14-seg digit) on Zen cape
#define DIGIT_ON_OFF_MASK (1 << 14)
// GPIO INput; p8.15 = pru0_pru_r31_15
//  = JSRT (Joystick Right) on Zen cape
#define JOYSTICK_RIGHT_MASK (1 << 15)
#define JOYSTICK_DOWN_MASK (1 << 14)

// Shared Memory Configuration
#define THIS_PRU_DRAM 0x00000 // Address of DRAM
#define OFFSET        0x200   // Skip 0x100 for Stack, 0x100 for Heap (from Makefile)

#define THIS_PRU_DRAM_USABLE (THIS_PRU_DRAM + OFFSET)

// NeoPixel Timing
#define oneCyclesOn     700/5   // Stay on 700ns
#define oneCyclesOff    800/5
#define zeroCyclesOn    350/5
#define zeroCyclesOff   600/5
#define resetCycles     60000/5 // Must be at least 50u, use 60u

#define NEOPIXEL_DATA_PIN 15

// This works for both PRU0 and PRU1 as both map their own memory to 0x0
volatile sharedMemStruct_t* pSharedMem = (volatile void*) THIS_PRU_DRAM_USABLE;

/// Set the pixels for each pixel in the NeoPixel based on the pixels array,
/// which is assumed to be of length NUM_PIXELS.
void setPixels(volatile uint32_t pixels[]) {
    __delay_cycles(resetCycles);

    for(int j = 0; j < NUM_PIXELS; j++) {
        for(int i = 31; i >= 0; i--) {
            if(pixels[j] & ((uint32_t)0x1 << i)) {
                __R30 |= 0x1 << NEOPIXEL_DATA_PIN;      // Set the GPIO pin to 1
                __delay_cycles(oneCyclesOn - 1);
                __R30 &= ~(0x1 << NEOPIXEL_DATA_PIN);   // Clear the GPIO pin
                __delay_cycles(oneCyclesOff - 2);
            } else {
                __R30 |= 0x1 << NEOPIXEL_DATA_PIN;      // Set the GPIO pin to 1
                __delay_cycles(zeroCyclesOn - 1);
                __R30 &= ~(0x1 << NEOPIXEL_DATA_PIN);   // Clear the GPIO pin
                __delay_cycles(zeroCyclesOff - 2);
            }
        }
    }

    // Send Reset
    __R30 &= ~(0x1 << NEOPIXEL_DATA_PIN);   // Clear the GPIO pin
    __delay_cycles(resetCycles);
}

// TODO:
void readJoystick(void) {
    if ((__R31 & JOYSTICK_RIGHT_MASK) != 0) {
        pSharedMem->Pru_joystickRight = 1;
    }
    else {
        pSharedMem->Pru_joystickRight = 0;
    }

    if ((__R31 & JOYSTICK_DOWN_MASK) != 0) {
        pSharedMem->Pru_joystickDown = 1;
    }
    else {
        pSharedMem->Pru_joystickDown = 0;
    }
}

void main(void) {
    // Zero everything out to begin with.
    memset((void*) pSharedMem->Linux_pixels, 0, sizeof(pSharedMem->Linux_pixels));
    pSharedMem->Pru_joystickDown = 0;
    pSharedMem->Pru_joystickRight = 0;

    while (true) {
        setPixels(pSharedMem->Linux_pixels);
        readJoystick();
    }
}
