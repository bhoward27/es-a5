// Module that contains joystick functions

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "joystick.h"
#include "utils.h"

#define INPUT_SLEEP_TIME 100
#define JOYSTICK_INPUT_DEBOUNCE_TIME 50

static pthread_t samplerId;
static volatile sharedMemStruct_t* pSharedPru0 = NULL;

// Learned how to make ms timer from this link: https://www.reddit.com/r/learnprogramming/comments/1dlxqv/comment/c9rksma/
static clock_t downDirectionTimer;
static clock_t rightDirectionTimer;

static bool cleanupFlag = false;

// The function below was provided by Dr. Brian Fraser
static void Joystick_runCommand(char *command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        // printf("--> %s", buffer);  // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf("  command:   %s\n", command);
        printf("  exit code: %d\n", exitCode);
    } 
}

static void *joystickThread(void *args)
{
	while (true) {
        sleepForMs(INPUT_SLEEP_TIME);

        if (cleanupFlag) {
            break;
        }

        if (pSharedPru0->Pru_joystickDown == 0 && JOYSTICK_INPUT_DEBOUNCE_TIME < ((double)(clock() - downDirectionTimer) / CLOCKS_PER_SEC * 1000)) {
            printf("Down Button Pressed\n");
            // Add logic here for fire
            downDirectionTimer = clock();
        } else if (pSharedPru0->Pru_joystickRight == 0 && JOYSTICK_INPUT_DEBOUNCE_TIME < ((double)(clock() - rightDirectionTimer) / CLOCKS_PER_SEC * 1000)) {
            printf("Right Button Pressed\n");
            // Add logic here for exit 
            rightDirectionTimer = clock();
        } 
    }
    return 0;
}

void Joystick_initializeJoystick(volatile sharedMemStruct_t* pSharedDataArg)
{
    pSharedPru0 = pSharedDataArg;
    Joystick_runCommand("config-pin p8.15 pruin");
    Joystick_runCommand("config-pin p8.16 pruin");
    
    downDirectionTimer = clock();
    rightDirectionTimer = clock();
    pthread_create(&samplerId, NULL, &joystickThread, NULL);
}

void Joystick_cleanupJoystick(void)
{
    cleanupFlag = true;
    pthread_join(samplerId, NULL);
}