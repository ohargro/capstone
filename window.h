/*--------------------------------
  Author:  Chris Dempewolf, Bryan Glazer, Orren Hargro
  Date:    Tuesday, January 24 2012
  File:    window.h
  Purpose:
---------------------------------*/

#include "base.h"

#ifndef WINDOW_H
#define WINDOW_H


class Window
{
    public:
        Window();
        void handle_window();
        void toggle_fullscreen();
        bool error();

    private:
        bool fullscreen;
        bool windowOK;
};

extern Window myWindow;

#endif // WINDOW_H
