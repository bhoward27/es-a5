/**
 * This file defines the bit patterns / waveforms for different colours on the NeoPixel.
 */
#ifndef COLOURS_H_
#define COLOURS_H_


#define OFF                0x00000000

#define GREEN              0x0f000000
#define RED                0x000f0000
#define BLUE               0x00000f00
#define WHITE              0x0000000f
#define RGB_WHITE          0x0f0f0f00
#define YELLOW             0x0f0f0000
#define PURPLE             0x000f0f00
#define TEAL               0x0f000f00

#define DIM_GREEN          0x02000000
#define DIM_RED            0x00020000
#define DIM_BLUE           0x00000200

#define BRIGHT_GREEN       0xff000000
#define BRIGHT_RED         0x00ff0000
#define BRIGHT_BLUE        0x0000ff00
#define BRIGHT_WHITE       0xffffff00

#define BRIGHT_WHITE_GREEN 0xff0000ff
#define BRIGHT_WHITE_RED   0x00ff00ff
#define BRIGHT_WHITE_BLUE  0x0000ffff
#define BRIGHT_WHITE_WHITE 0xffffffff

#endif