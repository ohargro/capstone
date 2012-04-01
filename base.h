/*--------------------------------
  Author:  Chris Dempewolf, Bryan Glazer, Orren Hargro
  Date:    Tuesday, January 31 2012
  File:    base.h
  Purpose: Implement standard functionality
---------------------------------*/

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

#ifndef BASE_H
#define BASE_H

//global variables & constant(s)
extern SDL_Surface* screen;
extern SDL_Surface* background;
extern SDL_Surface* startBackground;
extern SDL_Surface* tileSheet;
extern SDL_Surface* enemySheet;
extern SDL_Event event;
extern int screenWidth;
extern int screenHeight;
extern int levelWidth;
extern int levelHeight;
extern SDL_Rect camera;
extern int screenBPP;
extern Mix_Music* music;
extern const int FRAMES_PER_SECOND;

extern const int GRAVITY;
extern const int RIGHT;
extern const int LEFT;

//basic functions
bool load_files();
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* dest);
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect*);
bool init(int screenWidth, int screenHeight, int screenBPP);
SDL_Surface* load_image(std::string filename);
SDL_Surface* load_image(std::string filename, int red, int green, int blue);
SDL_Surface* load_font(std::string fontName, std::string text, SDL_Color color, int size);
bool checkCollision(SDL_Rect &a, SDL_Rect &b);
void handleMusic();
void cleanUp();

void startScreen();
void theGame();
void editLevelScreen();

#endif
