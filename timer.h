/*--------------------------------
  Author:  Chris Dempewolf
  Date:    Tuesday, January 24 2012
  File:    timer.h
  Purpose:
---------------------------------*/

#include "base.h"


#ifndef TIMER_H
#define TIMER_H

class Timer
{
    public:
        Timer();
        ~Timer();
        void start();
        void stop();
        void pause();
        void unpause();
        int get_ticks();
        bool is_started();
        bool is_paused();

    private:
        int startTicks;
        int pausedTicks;
        bool paused;
        bool started;
};

extern Timer fps;
extern Timer delta;

#endif
