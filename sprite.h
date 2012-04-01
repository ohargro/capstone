/*--------------------------------
  Author:  Chris Dempewolf, Bryan Glazer, Orren Hargro
  Date:    Tuesday, January 24 2012
  File:    sprite.h
  Purpose:
---------------------------------*/

#include "base.h"
#include "tile.h"
#include "enemy.h"

#ifndef SPRITE_H
#define SPRITE_H

class Sprite
{
    private:
        SDL_Rect box;
        float velX, velY;
        int acc; //acceleration
        bool inAir;
        SDL_Surface* sprite;
        SDL_Rect clipsRight[3]; //sprites for walking right
        SDL_Rect clipsLeft[3]; //sprites for walking left
        int frame; // frame of animation
        int status; //facing left or right

    public:
        Sprite(std::string filename);
        Sprite(std::string filename, int red, int green, int blue);
        ~Sprite();
        void clipMainCharacter();
        void handle_input();
        void move(Uint32 deltaTicks, Tile *tiles[], Enemy *enemies[]);
        void move(Tile *tiles[]);
        //bool collided(SDL_Rect a, SDL_Rect b);
        void show();
        void setCamera();
};

bool touchesWall(SDL_Rect box, Tile *tiles[]);
bool touchesEnemy(SDL_Rect box, Enemy *enemies[]);
#endif
