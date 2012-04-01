/*--------------------------------
  Author:  Chris Dempewolf, Bryan Glazer, Orren Hargro
  Date:    Tuesday, January 24 2012
  File:    window.cpp
  Purpose:
---------------------------------*/
#include "window.h"

Window myWindow;

Window::Window()
{
    //Set up the screen
    screen = SDL_SetVideoMode( screenWidth, screenHeight, screenBPP, SDL_DOUBLEBUF|SDL_HWSURFACE);

    if( screen == NULL )
    {
        windowOK = false;
        return;
    }
    else
        windowOK = true;

    fullscreen = false;
}

void Window::handle_window()
{
    //If there's something wrong with the window
    if( windowOK == false )
        return;

    else if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_RETURN))
        toggle_fullscreen();

    //if escape was pressed while in fullscreen
    else if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE) && (fullscreen == true))
        toggle_fullscreen();
}

void Window::toggle_fullscreen()
{
    if( fullscreen == false )
    {
        //Set the screen to fullscreen
        screen = SDL_SetVideoMode(screenWidth, screenHeight, screenBPP, SDL_DOUBLEBUF|SDL_HWSURFACE | SDL_FULLSCREEN );

        if( screen == NULL )
        {
            windowOK = false;
            return;
        }
        fullscreen = true;
    }
    else if(fullscreen == true)
    {
        //Window the screen
        screen = SDL_SetVideoMode( screenWidth, screenHeight, screenBPP, SDL_DOUBLEBUF|SDL_HWSURFACE);

        if( screen == NULL )
        {
            windowOK = false;
            return;
        }
        fullscreen = false;
    }
}

bool Window::error()
{
    return !windowOK;
}
