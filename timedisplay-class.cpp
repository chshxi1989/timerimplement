#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
#include "Timer.h"

Timer::Timer()
{
    m_timereach = false;
    m_ucsecond = 0;
}

Timer::~Timer()
{
    pthread_mutex_destroy(&m_timermutex);
}

void Timer::init()
{
    pthread_mutex_init(&m_timermutex, NULL);
    pthread_create(&m_tid, NULL, m_timerthread, this);
    pthread_detach(m_tid);
}

void Timer::setTimer(uint32_t ucsecond)
{
    pthread_mutex_lock(&m_timermutex);
    m_timereach = false;
    m_ucsecond = ucsecond;
    pthread_mutex_unlock(&m_timermutex);
}

bool Timer::getTimerState()
{
    return m_timereach;
}

void* Timer::m_timerthread(void* args)
{
    time_t oldtime;
    time_t newtime;
    Timer* _this = (Timer*)args;
    while(1)
    {
        if(_this->m_ucsecond > 0)
        {
             pthread_mutex_lock(&(_this->m_timermutex));
             oldtime = time(NULL);
             while(1)
             {
                 newtime = time(NULL);
                 if(newtime - oldtime >= _this->m_ucsecond)
                 {
                     _this->m_ucsecond = 0;
                     _this->m_timereach = true;
                     break;
                 }
             }
             pthread_mutex_unlock(&(_this->m_timermutex));
        }
    }
}

int main(int argc, char** args) {
    uint32_t ucsecond = 0;
    uint32_t ucminute = 0;
    uint32_t uchour = 0;
    uint32_t loop = 0;
    Timer timerInstance;
    timerInstance.init();
    while(1) {
        printf("%02d:%02d:%02d ", uchour, ucminute, ucsecond);
        timerInstance.setTimer(1);
        while(timerInstance.getTimerState() != true);
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
}
