#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

class Timer
{
    public:
        Timer();

        void StartTime();
        void EndTime();

        float TotalSeconds();
        float Fps();

        virtual ~Timer();

    protected:
    private:
        Timer(const Timer& other) {}
        void operator=(const Timer& other) {}

        float seconds;
        float fps;
        Uint64 startTime;
        Uint64 endTime;
        Uint64 lastEndTime;
        Uint64 freqTime;
};

#endif // hpTIMER_H
