/*--------------------------------
  Author:  Chris Dempewolf, Bryan Glazer, Orren Hargro
  Date:    Tuesday, January 31 2012
  File:    tile.h
  Purpose:
---------------------------------*/

#include "base.h"

#ifndef TILE_H
#define TILE_H

//Tile constants
extern const int TILE_WIDTH;
extern const int TILE_HEIGHT;
extern const int TOTAL_TILES;
extern const int NUM_TILE_SPRITES;
extern SDL_Rect tileClips[];

//The different tile sprites
extern const int TILE_RED;
extern const int TILE_GREEN;
extern const int TILE_BLUE;
extern const int TILE_CENTER;
extern const int TILE_TOP;
extern const int TILE_TOPRIGHT;
extern const int TILE_RIGHT;
extern const int TILE_BOTTOMRIGHT;
extern const int TILE_BOTTOM;
extern const int TILE_BOTTOMLEFT;
extern const int TILE_LEFT;
extern const int TILE_TOPLEFT;

class Tile
{
    private:
        SDL_Rect box; // the attributes of the tile
        int type; // the tile type

    public:
        Tile(int x, int y, int tileType);
        void show();
        int getType();
        SDL_Rect &getBox(); // get the collision box
};

void clipTiles();
bool setTiles(Tile *tiles[]);
bool loadTiles(Tile *tiles[]);
void setEditorCamera();
void showType(int tileType);
void putTile(Tile *tiles[], int tileType);
void saveTiles(Tile *tiles[]);
void deleteTiles(Tile *tiles[]);

#endif // TILE_H
