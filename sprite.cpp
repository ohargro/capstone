/*--------------------------------
  Author:  Chris Dempewolf, Bryan Glazer, Orren Hargro
  Date:    Tuesday, January 24 2012
  File:    sprite.cpp
  Purpose:
---------------------------------*/
#include "sprite.h"

Sprite::Sprite(std::string filename)
{
    sprite = load_image(filename);
    box.x = 0;
    box.y = 0;
    box.w = sprite->w;
    box.h = sprite->h;
    velY = GRAVITY;
    velX = 0;
    acc = 300;
    inAir = true;

    status = 0;
    frame = 0;

    clipMainCharacter();
}

//Load sprite and set width and height and set pixels that are equal to RGB to be transparent
Sprite::Sprite(std::string filename, int red, int green, int blue)
{
    sprite = load_image(filename, red, green, blue);

    box.x = 0;
    box.y = 0;
    box.w = 34;
    box.h = 69;

    velY = GRAVITY; //downward force
    velX = 0;
    acc = 300;
    inAir = true;

    status = RIGHT;
    frame = 1;

    clipMainCharacter();
}

Sprite::~Sprite()
{
    SDL_FreeSurface(sprite);
}

void Sprite::clipMainCharacter()
{
    // clip the individual character sprites from the loaded image
    clipsRight[0].x = 18;
    clipsRight[0].y = 123;
    clipsRight[0].w = 36;
    clipsRight[0].h = 69;

    clipsRight[1].x = 90;
    clipsRight[1].y = 120;
    clipsRight[1].w = 33;
    clipsRight[1].h = 72;

    clipsRight[2].x = 162;
    clipsRight[2].y = 123;
    clipsRight[2].w = 36;
    clipsRight[2].h = 69;

    clipsLeft[0].x = 18;
    clipsLeft[0].y = 315;
    clipsLeft[0].w = 36;
    clipsLeft[0].h = 69;

    clipsLeft[1].x = 93;
    clipsLeft[1].y = 312;
    clipsLeft[1].w = 33;
    clipsLeft[1].h = 72;

    clipsLeft[2].x = 161;
    clipsLeft[2].y = 315;
    clipsLeft[2].w = 36;
    clipsLeft[2].h = 69;
}

void Sprite::handle_input()
{
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_d: case SDLK_RIGHT: velX += acc; break;
            case SDLK_a: case SDLK_LEFT: velX -= acc; break;
            case SDLK_w: case SDLK_UP: velY -= acc; break;
            case SDLK_s: case SDLK_DOWN: velY += acc; break;
            default:;
        }
    }
    else if(event.type == SDL_KEYUP)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_d: case SDLK_RIGHT: velX -= acc; break;
            case SDLK_a: case SDLK_LEFT: velX += acc; break;
            case SDLK_w: case SDLK_UP: velY += acc; break;
            case SDLK_s: case SDLK_DOWN: velY -= acc; break;
            default:;
        }
    }
}

// this move method allows for more precise movement
void Sprite::move(Uint32 deltaTicks, Tile *tiles[], Enemy *enemies[])
{
    float prevX = box.x;
    float prevY = box.y;

    //Move the sprite left or right
    box.x += velX * (deltaTicks / 1000.f);

    //If the sprite went too far to the left or right or touched a wall
    if( ( box.x < 0 ) || ( box.x + box.w > levelWidth ))
        box.x -= velX * (deltaTicks / 1000.f);
    else if (touchesWall(box, tiles) || (touchesEnemy(box, enemies)))
        box.x = prevX;

    //Move the sprite up or down
    box.y += velY * (deltaTicks / 1000.f);

    //If the sprite went too far up or down or touched a wall
    if( ( box.y < 0 ) || ( box.y + box.h > levelHeight ))
        box.y -= velY * (deltaTicks / 1000.f);
    else if (touchesWall(box, tiles) || (touchesEnemy(box, enemies)))
        box.y = prevY;
}

void Sprite::move( Tile *tiles[] )
{
    //Move the sprite left or right
    box.x += velX;

    //If the sprite went too far to the left or right or touched a wall
    if( ( box.x < 0 ) || ( box.x + box.w > levelWidth ) || (touchesWall(box, tiles)))
        box.x -= velX;

    //Move the sprite up or down
    box.y += velY;

    //If the sprite went too far up or down or touched a wall
    if( ( box.y < 0 ) || ( box.y + box.w > levelHeight ) || (touchesWall(box, tiles)) )
        box.y -= velY;
}

// can test if Rect a is on top of Rect b, not currently implemented anywhere
// contains algorithm for determining if sprite is on ground or not
/*bool Sprite::collided(SDL_Rect a, SDL_Rect b)
{
    int leftA = a.x;
    int leftB = b.x;
    int rightA = a.x + a.w;
    int rightB = b.x + b.w;
    int topA = a.y;
    int topB = b.y;
    int botA = a.y + a.h;
    int botB = b.y + b.h;

    SDL_Color c = {255, 255, 255};
    SDL_Surface* m = NULL;

    //A is on top of B (with some leeway up/down)
    if ((botA >= topB - 3) && ((botA <= topB + 3)) && (rightA >= leftB) && (leftA <= rightB))
        inAir = false;
    else
        inAir = true;

    if (inAir == false)
    {
        m = load_font("eva.ttf", "Ground", c, 22);
        apply_surface(0, 25, m, screen);
    }
    else
    {
        m = load_font("eva.ttf", "Air", c, 22);
        apply_surface(0, 0, m, screen);
    }
    SDL_FreeSurface(m);

    if((botA <= topB) || (topA >= botB) || (rightA <= leftB) || (leftA >= rightB))
        return false;

    return true;
}*/

void Sprite::show()
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

    if (status == RIGHT)
        apply_surface((int)box.x - camera.x, (int)box.y - camera.y, sprite, screen, &clipsRight[frame]);
    else if (status == LEFT)
        apply_surface((int)box.x - camera.x, (int)box.y - camera.y, sprite, screen, &clipsLeft[frame]);
    else //basic show sprite
        apply_surface((int)box.x - camera.x, (int)box.y - camera.y, sprite, screen);
}

//center the camera over the sprite
void Sprite::setCamera()
{
    camera.x = (box.x + 0) - screenWidth / 2;
    camera.y = (box.y + 0) - screenHeight / 2;

    //keep camera in bounds
    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > levelWidth - camera.w)
        camera.x = levelWidth - camera.w;
    if (camera.y > levelHeight - camera.h)
        camera.y = levelHeight - camera.h;
}

bool touchesWall(SDL_Rect box, Tile *tiles[])
{
    //Go through the tiles
    for(int t = 0; t < TOTAL_TILES; t++)
    {
        //If the tile is a wall type tile
        if((tiles[t]->getType() >= TILE_CENTER) && (tiles[t]->getType() <= TILE_TOPLEFT))
        {
            //If the collision box touches the wall tile
            if(checkCollision(box, tiles[ t ]->getBox()) == true)
                return true;
        }
    }

    //If no wall tiles were touched
    return false;
}

bool touchesEnemy(SDL_Rect box, Enemy *enemies[])
{
    for (int e = 0; e < TOTAL_ENEMIES; e++)
    {
        if (checkCollision(box, enemies[e]->getBox()) == true)
            return true;
    }

    // if no enemies were touched
    return false;
}
