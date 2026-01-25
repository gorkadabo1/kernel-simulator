#include "clock_timer.h"
#include "pthread.h"
#include "commons.h"
#include <stdio.h>

pthread_cond_t tick_cond;

int clock_cycles = 0;
void* clock_function() {
    while (1) {
        //printf("Clock: Emitiendo pulso.\n");
        pthread_cond_signal(&beat);
        clock_cycles++;
    }

    pthread_cond_signal(&beat);

    return NULL;
}

void* timer_function() {
    int pulseCount = 0;
    while (1) {
        pthread_cond_wait(&beat, &lock);

        pulseCount++;
        if (pulseCount == config.intervalo_timer) {
            //printf("Timer: Emitiendo tick.\n");
            pthread_cond_signal(&tick_cond);
            pulseCount = 0;
        }
    }
    return NULL;
}
