/**
 * Provide functions related to the buzzer, such as:
 *      - Initializing the buzzer module
 *      - Playing a miss sound from the buzzer
 *      - Playing a hit sound from the buzzer
 *      - Cleaning up the buzzer module
*/

#ifndef BUZZER_H
#define BUZZER_H

void Buzzer_initializeBuzzer(void);
void Buzzer_playMissSound(void);
void Buzzer_playHitSound(void);
void Buzzer_cleanupBuzzer(void);

#endif