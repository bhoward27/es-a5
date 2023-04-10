#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "sharedDataStruct.h"
#include "colours.h"
#include "accelerometer.h"
#include "log.h"

// General PRU Memory Sharing Routine
// ----------------------------------
#define PRU_ADDR         0x4A300000 // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN          0x80000    // Length of PRU memory
#define PRU0_DRAM        0x00000    // Offset to DRAM
#define PRU1_DRAM        0x02000
#define PRU_SHARED_MEM   0x10000    // Offset to shared memory
#define PRU_MEM_RESERVED 0x200      // Amount used by stack and heap

// Convert base address to each memory section
#define PRU0_MEM_FROM_BASE(base) ((base) + PRU0_DRAM + PRU_MEM_RESERVED)
#define PRU1_MEM_FRMO_BASE(base) ((base) + PRU1_DRAM + PRU_MEM_RESERVED)
#define PRU_SHARED_MEM_FROM_BASE(base) ((base) + PRU_SHARED_MEM)

// Return the address of the PRU's base memory.
volatile void* getPruMmapAddr(void) {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("ERROR: could not open /dev/mem");
        exit(EXIT_FAILURE);
    }

    // Points to start of PRU memory.
    volatile void* pPruBase = mmap(0,
                                   PRU_LEN, PROT_READ | PROT_WRITE,
                                   MAP_SHARED,
                                   fd,
                                   PRU_ADDR);
    if (pPruBase == MAP_FAILED) {
        perror("ERROR: could not map memory");
        exit(EXIT_FAILURE);
    }
    close(fd);
    return pPruBase;
}

void freePruMmapAddr(volatile void* pPruBase) {
    if (munmap((void*) pPruBase, PRU_LEN)) {
        perror("PRU munmap failed");
        exit(EXIT_FAILURE);
    }
}

void setAllPixels(volatile uint32_t pixels[], uint32_t colour) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pixels[i] = colour;
    }
}

int main(void) {
    printf("Hello world!\n");

    initLogLevel();

    // Get access to shared memory for my uses.
    volatile void* pPruBase = getPruMmapAddr();
    volatile sharedMemStruct_t* pSharedPru0 = PRU0_MEM_FROM_BASE(pPruBase);

    Accelerometer_init(pSharedPru0);

    // setAllPixels(pSharedPru0->Linux_pixels, YELLOW);
    // sleep(5);
    // setAllPixels(pSharedPru0->Linux_pixels, RED);
    // sleep(5);
    // setAllPixels(pSharedPru0->Linux_pixels, OFF);

    Accelerometer_waitForShutdown();

    // Cleanup
    freePruMmapAddr(pPruBase);
}