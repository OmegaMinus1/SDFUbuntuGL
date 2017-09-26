#include "timer.h"

Timer::Timer()
{
    freqTime = SDL_GetPerformanceFrequency();
}

Timer::~Timer()
{

}

void Timer::StartTime()
{
    startTime = SDL_GetPerformanceCounter();
}

void Timer::EndTime()
{
    lastEndTime = endTime;
    endTime = SDL_GetPerformanceCounter();
}

float Timer::TotalSeconds()
{
    seconds = (float)(endTime - startTime) / (float)freqTime;

    return seconds;
}

float Timer::Fps()
{
    fps = (float)freqTime / (float)(endTime - lastEndTime);

    return fps;
}
