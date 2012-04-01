/*--------------------------------
  Author:  Chris Dempewolf, Bryan Glazer, Orren Hargro
  Date:    Tuesday, January 24 2012
  File:    enemy.h
  Purpose:
---------------------------------*/

#include "base.h"
#include "tile.h"

#ifndef ENEMY_H
#define ENEMY_H

// enemy constants
extern const int ENEMY_WIDTH;
extern const int ENEMY_HEIGHT;
extern const int TOTAL_ENEMIES; //num of enemies alive
extern const int NUM_ENEMY_SPRITES; // different types of enemies
extern SDL_Rect enemyClips[];

extern const int ENEMY_ROBOT;
extern const int ENEMY_ANDROID;

class Enemy
{
    private:
        SDL_Rect box; //collision box
        int type;
        float velX, velY;
        SDL_Rect clipsRight[3];
        SDL_Rect clipsLeft[3];
        int frame; //frame of animation
        int status; //facing left or right

    public:
        Enemy(int x, int y, int enemyType);
        SDL_Rect &getBox();
        void walk(Uint32 deltaTicks, Tile *tiles[]);
        void show();
        bool touchWall( SDL_Rect box, Tile *tiles[] );

        int getType();
        void setVelX(int velocity);
        float getVelX();

        void clipRobot();
        void clipAndroid();
};

void deleteEnemies(Enemy *enemies[]);

#endif // ENEMY_H
