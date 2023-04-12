// Module that contains buzzer functions

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "utils.h"
#include "shutdown.h"
#include "accelerometer.h"
#include "target.h"

#define ENABLE_FILE "/dev/bone/pwm/0/a/enable"
#define DUTY_CYCLE_FILE "/dev/bone/pwm/0/a/duty_cycle"
#define PERIOD_FILE "/dev/bone/pwm/0/a/period"


static pthread_t samplerId;
static bool missSoundFlag = false;
static bool hitSoundFlag = false;

// The function below was provided by Dr. Brian Fraser
static void Buzzer_runCommand(char *command)
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

static void Buzzer_writeToFile(char *fileToWrite, char *string)
{
    FILE *file = fopen(fileToWrite, "w");
    if (file == NULL) {
        printf("ERROR: Unable to open direction file.\n");
        exit(1);
    }
    fprintf(file, "%s", string);
    fclose(file);
}

static void playMissSound() {
    Buzzer_writeToFile(PERIOD_FILE, "10204080");
    Buzzer_writeToFile(DUTY_CYCLE_FILE, "5102040");
    Buzzer_writeToFile(ENABLE_FILE, "1");
    sleepForMs(100);
    Buzzer_writeToFile(PERIOD_FILE, "11453440");
    Buzzer_writeToFile(DUTY_CYCLE_FILE, "5726720");
    sleepForMs(200);
    Buzzer_writeToFile(PERIOD_FILE, "6179705");
    Buzzer_writeToFile(DUTY_CYCLE_FILE, "3089852");
    Buzzer_writeToFile(ENABLE_FILE, "1");
    sleepForMs(100);
    Buzzer_writeToFile(PERIOD_FILE, "12134448");
    Buzzer_writeToFile(DUTY_CYCLE_FILE, "6067224");
    sleepForMs(200);
    Buzzer_writeToFile(DUTY_CYCLE_FILE, "0");
    Buzzer_writeToFile(ENABLE_FILE, "0");
}

static void playHitSound() {
    Buzzer_writeToFile(PERIOD_FILE, "955564");
    Buzzer_writeToFile(DUTY_CYCLE_FILE, "477782");
    Buzzer_writeToFile(ENABLE_FILE, "1");
    sleepForMs(100);
    Buzzer_writeToFile(PERIOD_FILE, "851310");
    Buzzer_writeToFile(DUTY_CYCLE_FILE, "425655");
    sleepForMs(200);
    Buzzer_writeToFile(PERIOD_FILE, "568180");
    Buzzer_writeToFile(DUTY_CYCLE_FILE, "284090");
    sleepForMs(200);
    Buzzer_writeToFile(DUTY_CYCLE_FILE, "0");
    Buzzer_writeToFile(ENABLE_FILE, "0");
}

void Buzzer_playMissSound(void) {
    missSoundFlag = true;
}

void Buzzer_playHitSound(void){
    hitSoundFlag = true;
}

static void *buzzerThread(void *args)
{
	while (!isShutdownRequested()) {
        if (missSoundFlag) {
            playMissSound();
            missSoundFlag = false;
        }

        if (hitSoundFlag) {
            playHitSound();
            hitSoundFlag = false;
        }
        sleepForMs(25);
    }
    return 0;
}

void Buzzer_initializeBuzzer(void)
{
    Buzzer_runCommand("sudo config-pin p9_22 pwm");

    pthread_create(&samplerId, NULL, &buzzerThread, NULL);
}

void Buzzer_cleanupBuzzer(void)
{
    pthread_join(samplerId, NULL);
    Buzzer_writeToFile(ENABLE_FILE, "0");
}