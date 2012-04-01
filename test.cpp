/*--------------------------------
  Author:  Chris Dempewolf, Bryan Glazer, Orren Hargro
  Date:    Tuesday, January 24 2012
  File:    test.cpp
  Purpose: Learning SDL
---------------------------------*/
#include "timer.h"
#include "sprite.h"
#include "enemy.h"
#include "window.h"
#include "base.h"
#include <fstream>

int main(int argc, char* args[])
{
    if(!init(screenWidth, screenHeight, screenBPP))
        return (EXIT_FAILURE);

    //Window myWindow; //global instance declared in window.cpp
    //Timer fps; //global instance declared in timer.cpp

    if (load_files() == false)
        return (EXIT_FAILURE);

    clipTiles();

    startScreen();

    cleanUp();

    return (EXIT_SUCCESS);
}
