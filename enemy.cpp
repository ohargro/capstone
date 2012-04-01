/*--------------------------------
  Author:  Chris Dempewolf, Bryan Glazer, Orren Hargro
  Date:    Tuesday, January 24 2012
  File:    enemy.cpp
  Purpose:
---------------------------------*/

#include "enemy.h"

const int ENEMY_WIDTH = 33;
const int ENEMY_HEIGHT = 63;
const int TOTAL_ENEMIES = 3; // num of enemies alive
const int NUM_ENEMY_SPRITES = 2; // different types of enemies
SDL_Rect enemyClips[TOTAL_ENEMIES]; //holds a clip of each tile, used for editor

const int ENEMY_ROBOT = 0;
const int ENEMY_ANDROID = 1;

Enemy::Enemy(int x, int y, int enemyType)
{
    if (enemyType == ENEMY_ROBOT)
    {
        clipRobot();
    }
    else if (enemyType == ENEMY_ANDROID)
    {
        clipAndroid();
    }

    type = enemyType;

    box.x = x;
    box.y = y;
    box.w = ENEMY_WIDTH;
    box.h = ENEMY_HEIGHT;

    velY = GRAVITY; //downward force
    velX = 0;

    status = RIGHT;
    frame = 1;
}

SDL_Rect &Enemy::getBox()
{
    return box;
}

void Enemy::walk(Uint32 deltaTicks, Tile *tiles[])
{
    float prevX = box.x;
    float prevY = box.y;

    //Move the enemy forward
    box.x += velX * (deltaTicks / 1000.f);

    //If the enemy went too far to the left or right or touched a wall
    if( ( box.x < 0 ) || ( box.x + box.w > levelWidth ))
        box.x -= velX * (deltaTicks / 1000.f);
    else if (touchWall( box, tiles ))
        box.x = prevX;

    //apply gravity
    box.y += velY * (deltaTicks / 1000.f);

    //If the enemy went too far up or down or touched a wall
    if( ( box.y < 0 ) || ( box.y + box.h > levelHeight ))
        box.y -= velY * (deltaTicks / 1000.f);
    else if (touchWall( box, tiles ))
        box.y = prevY;
}

void Enemy::show()
{
    if (velX < 0) //moving left
    {
        status = LEFT;
        frame++;
    }
    else if (velX > 0) //moving right
    {
        status = RIGHT;
        frame++;
    }
    else //velX == 0
        frame = 1;

    if (frame >= 3) // loop frames
        frame = 0;

    if(checkCollision(camera, box) == true) //if enemy is within the camera
    {
        if (status == RIGHT)
            apply_surface((int)box.x - camera.x, (int)box.y - camera.y, enemySheet, screen, &clipsRight[frame]);
        else if (status == LEFT)
            apply_surface((int)box.x - camera.x, (int)box.y - camera.y, enemySheet, screen, &clipsLeft[frame]);
    }
}

int Enemy::getType()
{
    return type;
}

void Enemy::setVelX(int velocity)
{
    velX = velocity;
}

float Enemy::getVelX()
{
    return velX;
}

// checks to see if enemy touched a wall
// if the enemy touches a wall to the left or to the right, it turns around
bool Enemy::touchWall(SDL_Rect box, Tile *tiles[])
{
    int leftA = box.x;
    int rightA = box.x + box.w;
    int botA = box.y + box.h;

    //Go through the tiles
    for(int t = 0; t < TOTAL_TILES; t++)
    {
        int leftB = tiles[t]->getBox().x;
        int rightB = tiles[t]->getBox().x + tiles[t]->getBox().w;
        int botB = tiles[t]->getBox().y + tiles[t]->getBox().h;

        //If the tile is a wall type tile
        if((tiles[t]->getType() >= TILE_CENTER) && (tiles[t]->getType() <= TILE_TOPLEFT))
        {
            //If the collision box touches the wall tile
            if( checkCollision( box, tiles[ t ]->getBox() ) == true )
            {
                //if the wall tile was directly right or left of the enemy (plus some leeway)
                if (((leftA <= rightB + 6) || (rightA >= leftB - 6)) && (botA >= botB - 6))
                {
                    velX *= -1; // turn around
                }
                return true;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}

void Enemy::clipRobot()
{
    // clip the individual robot sprites from the loaded image
    clipsRight[0].x = 7;
    clipsRight[0].y = 9;
    clipsRight[0].w = 36;
    clipsRight[0].h = 63;

    clipsRight[1].x = 85;
    clipsRight[1].y = 6;
    clipsRight[1].w = 27;
    clipsRight[1].h = 66;

    clipsRight[2].x = 151;
    clipsRight[2].y = 9;
    clipsRight[2].w = 39;
    clipsRight[2].h = 63;

    clipsLeft[0].x = 208;
    clipsLeft[0].y = 8;
    clipsLeft[0].w = 39;
    clipsLeft[0].h = 63;

    clipsLeft[1].x = 286;
    clipsLeft[1].y = 5;
    clipsLeft[1].w = 29;
    clipsLeft[1].h = 66;

    clipsLeft[2].x = 355;
    clipsLeft[2].y = 8;
    clipsLeft[2].w = 36;
    clipsLeft[2].h = 63;
}

void Enemy::clipAndroid()
{
    // clip the individual robot sprites from the loaded image
    clipsRight[0].x = 11;
    clipsRight[0].y = 91;
    clipsRight[0].w = 33;
    clipsRight[0].h = 63;

    clipsRight[1].x = 83;
    clipsRight[1].y = 88;
    clipsRight[1].w = 30;
    clipsRight[1].h = 66;

    clipsRight[2].x = 155;
    clipsRight[2].y = 91;
    clipsRight[2].w = 33;
    clipsRight[2].h = 63;

    clipsLeft[0].x = 212;
    clipsLeft[0].y = 90;
    clipsLeft[0].w = 33;
    clipsLeft[0].h = 63;

    clipsLeft[1].x = 287;
    clipsLeft[1].y = 87;
    clipsLeft[1].w = 30;
    clipsLeft[1].h = 66;

    clipsLeft[2].x = 356;
    clipsLeft[2].y = 90;
    clipsLeft[2].w = 33;
    clipsLeft[2].h = 63;
}

void deleteEnemies(Enemy *enemies[])
{
    for( int e = 0; e < TOTAL_ENEMIES; e++ )
    {
        delete enemies[e];
    }
}
