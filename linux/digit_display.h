// This module drives the 14 segment display.
#ifndef DIGIT_DISPLAY_H_
#define DIGIT_DISPLAY_H_

#include "gpio.h"

#define DIGIT_DISPLAY_GPIO_HEADER "p8"
#define DIGIT_DISPLAY_LEFT_GPIO_PIN "26"
#define DIGIT_DISPLAY_RIGHT_GPIO_PIN "27"

#define DIGIT_DISPLAY_LEFT_GPIO_LINUX_PIN "61"
#define DIGIT_DISPLAY_RIGHT_GPIO_LINUX_PIN "44"

#define DIGIT_DISPLAY_I2C_BUS_NUMBER 1
// NOTE: Addresses would be different on Zen Cape Red.
#define DIGIT_DISPLAY_TOP_I2C_REGISTER_ADDRESS 0x15
#define DIGIT_DISPLAY_BOTTOM_I2C_REGISTER_ADDRESS 0x14

// ******* BIT PATTERNS *********
#define DIGIT_DISPLAY_PATTERN_ALL_OFF                     0b00000000
#define DIGIT_DISPLAY_PATTERN_ALL_ON                      0b11111111

// Bit patterns controlling a single segment of the top half.
#define DIGIT_DISPLAY_PATTERN_TOP_RIGHT_VERTICAL          0b00000010
#define DIGIT_DISPLAY_PATTERN_TOP_HORIZONTAL              0b00000100
#define DIGIT_DISPLAY_PATTERN_TOP_MID_LEFT_HORIZONTAL     0b00001000
#define DIGIT_DISPLAY_PATTERN_TOP_RIGHT_DIAGONAL          0b00010000
#define DIGIT_DISPLAY_PATTERN_TOP_MID_VERTICAL            0b00100000
#define DIGIT_DISPLAY_PATTERN_TOP_LEFT_DIAGONAL           0b01000000
#define DIGIT_DISPLAY_PATTERN_TOP_LEFT_VERTICAL           0b10000000

// Bit patterns controlling a single segment of the bottom half.
#define DIGIT_DISPLAY_PATTERN_BOTTOM_LEFT_VERTICAL        0b00000001
#define DIGIT_DISPLAY_PATTERN_BOTTOM_LEFT_DIAGIONAL       0b00000010
#define DIGIT_DISPLAY_PATTERN_BOTTOM_MID_VERTICAL         0b00000100
#define DIGIT_DISPLAY_PATTERN_BOTTOM_RIGHT_DIAGONAL       0b00001000
#define DIGIT_DISPLAY_PATTERN_BOTTOM_MID_RIGHT_HORIZONTAL 0b00010000
#define DIGIT_DISPLAY_PATTERN_BOTTOM_HORIZONTAL           0b00100000
#define DIGIT_DISPLAY_PATTERN_BOTTOM_DOT                  0b01000000
#define DIGIT_DISPLAY_PATTERN_BOTTOM_RIGHT_VERTICAL       0b10000000

typedef struct {
    uint8 topBitPattern;
    uint8 bottomBitPattern;
} DigitDisplayPattern;

extern const DigitDisplayPattern DigitDisplay_digitPatterns[10];
// ************** END OF BIT PATTERNS ****************

#define DIGIT_DISPLAY_MAX_NUM 99

extern const GpioLinuxInfo DigitDisplay_gpioLeft;
extern const GpioLinuxInfo DigitDisplay_gpioRight;

typedef enum {
    DIGIT_DISPLAY_LEFT,
    DIGIT_DISPLAY_RIGHT
} DigitDisplayHalf;

void DigitDisplay_init(void);
void DigitDisplay_waitForShutdown(void);

#endif