#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

class Timer
{
public:
    Timer();
    ~Timer();
    void init();
    void setTimer(uint32_t ucsecond);
    bool getTimerState();
private:
    bool m_timereach;
    uint32_t m_ucsecond;
    pthread_t m_tid;
    pthread_mutex_t m_timermutex;
    static void* m_timerthread(void* args);
};
