/*--------------------------------
  Author:  Chris Dempewolf
  Date:    Tuesday, January 24 2012
  File:    timer.cpp
  Purpose:
---------------------------------*/
#include "timer.h"

Timer fps;
Timer delta;

Timer::Timer()
{
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

Timer::~Timer()
{

}

void Timer::start()
{
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    started = false;
    paused = false;
}

void Timer::pause()
{
    if(started && !paused)
    {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    if(paused)
    {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //Timer started
    if(started)
    {
        if(paused)
            return pausedTicks;
        else
            return SDL_GetTicks() - startTicks;
    }

    //Timer not started
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}
