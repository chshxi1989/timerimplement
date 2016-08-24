#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
bool isTimer = false;
uint32_t g_ucsecond = 0;
bool bTImerStart = false;
pthread_mutex_t timermutex;

void* timercounter(void* p) {
    time_t old;
    time_t new;
    while(1) {
        if(bTImerStart) {
            pthread_mutex_lock(&timermutex);
            old = time(NULL);
            while(1) {
                new = time(NULL);
                if(new - old >= g_ucsecond) {
                    bTImerStart = false;
                    isTimer = true;
                    break;
                }
            }
            pthread_mutex_unlock(&timermutex);
        }
    }
}

bool GetTimerStatus() {
    return isTimer;
}

int settimer(uint32_t ucsecond) {
    pthread_mutex_lock(&timermutex);
    isTimer= false;
    bTImerStart = true;
    g_ucsecond = ucsecond;
    pthread_mutex_unlock(&timermutex);
    return 0;
}

int main(int argc, char** args) {
    uint32_t ucsecond = 0;
    uint32_t ucminute = 0;
    uint32_t uchour = 0;
    pthread_t tid;
    uint32_t loop = 0;
    pthread_mutex_init(&timermutex,NULL);
    pthread_create(&tid, NULL, timercounter, NULL);
    pthread_detach(tid);
    while(1) {
        printf("%02d:%02d:%02d ", uchour, ucminute, ucsecond);
        settimer(1);
        while(GetTimerStatus() != true);
        ucsecond++;
        if(ucsecond == 60) {
            ucsecond = 0;
            ucminute++;
        }
        if(ucminute == 60) {
            ucminute = 0;
            uchour++;
        }
        if(uchour == 60) {
            uchour =0;
        }
        loop = loop%185;
        if (loop < 60) {
            printf("100\n");  // red ON, green OFF, yellow OFF
        } else if(loop >= 60 && loop < 180) {
            printf("010\n");  // red OFF, green ON, yellow OFF
        } else {
            if (loop%2 == 0) {
                printf("001\n"); // red OFF, green OFF, yellow ON
            } else {
                printf("000\n"); // red OFF, green OFF, yellow OFF
            }
        }
        loop++;
   }
   pthread_mutex_destroy(&timermutex);
}
