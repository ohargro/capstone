/*--------------------------------
  Author:  Chris Dempewolf, Bryan Glazer, Orren Hargro
  Date:    Tuesday, January 31 2012
  File:    tile.cpp
  Purpose:
---------------------------------*/

#include "tile.h"

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int NUM_TILE_SPRITES = 12;
SDL_Rect tileClips[TOTAL_TILES]; //holds a clip of each tile

//The different tile sprites
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

Tile::Tile( int x, int y, int tileType )
{
    //Get the offsets
    box.x = x;
    box.y = y;

    //Set the collision box
    box.w = TILE_WIDTH;
    box.h = TILE_HEIGHT;

    //Get the tile type
    type = tileType;
}

void Tile::show()
{
    //If the tile is on screen
    if( checkCollision( camera, box ) == true )
    {
        //Show the tile
        apply_surface( box.x - camera.x, box.y - camera.y, tileSheet, screen, &tileClips[ type ] );

    }
}

int Tile::getType()
{
    return type;
}

SDL_Rect &Tile::getBox() // get collision box
{
    return box;
}

void clipTiles() // clip the tile sprites from the tile image
{
    //Clip the sprite sheet
    tileClips[ TILE_RED ].x = 0;
    tileClips[ TILE_RED ].y = 0;
    tileClips[ TILE_RED ].w = TILE_WIDTH;
    tileClips[ TILE_RED ].h = TILE_HEIGHT;

    tileClips[ TILE_GREEN ].x = 0;
    tileClips[ TILE_GREEN ].y = 80;
    tileClips[ TILE_GREEN ].w = TILE_WIDTH;
    tileClips[ TILE_GREEN ].h = TILE_HEIGHT;

    tileClips[ TILE_BLUE ].x = 0;
    tileClips[ TILE_BLUE ].y = 160;
    tileClips[ TILE_BLUE ].w = TILE_WIDTH;
    tileClips[ TILE_BLUE ].h = TILE_HEIGHT;

    tileClips[ TILE_TOPLEFT ].x = 80;
    tileClips[ TILE_TOPLEFT ].y = 0;
    tileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
    tileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

    tileClips[ TILE_LEFT ].x = 80;
    tileClips[ TILE_LEFT ].y = 80;
    tileClips[ TILE_LEFT ].w = TILE_WIDTH;
    tileClips[ TILE_LEFT ].h = TILE_HEIGHT;

    tileClips[ TILE_BOTTOMLEFT ].x = 80;
    tileClips[ TILE_BOTTOMLEFT ].y = 160;
    tileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
    tileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

    tileClips[ TILE_TOP ].x = 160;
    tileClips[ TILE_TOP ].y = 0;
    tileClips[ TILE_TOP ].w = TILE_WIDTH;
    tileClips[ TILE_TOP ].h = TILE_HEIGHT;

    tileClips[ TILE_CENTER ].x = 160;
    tileClips[ TILE_CENTER ].y = 80;
    tileClips[ TILE_CENTER ].w = TILE_WIDTH;
    tileClips[ TILE_CENTER ].h = TILE_HEIGHT;

    tileClips[ TILE_BOTTOM ].x = 160;
    tileClips[ TILE_BOTTOM ].y = 160;
    tileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
    tileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;

    tileClips[ TILE_TOPRIGHT ].x = 240;
    tileClips[ TILE_TOPRIGHT ].y = 0;
    tileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
    tileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

    tileClips[ TILE_RIGHT ].x = 240;
    tileClips[ TILE_RIGHT ].y = 80;
    tileClips[ TILE_RIGHT ].w = TILE_WIDTH;
    tileClips[ TILE_RIGHT ].h = TILE_HEIGHT;

    tileClips[ TILE_BOTTOMRIGHT ].x = 240;
    tileClips[ TILE_BOTTOMRIGHT ].y = 160;
    tileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
    tileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
}

// load tiles for gameplay
bool setTiles(Tile *tiles[])
{
    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map("tiling.map");

    //If the map couldn't be loaded
    if( map == NULL )
    {
        return false;
    }

    //Initialize the tiles
    for( int t = 0; t < TOTAL_TILES; t++ )
    {
        //Determines what kind of tile will be made
        int tileType = -1;

        //Read tile from map file
        map >> tileType;

        //If the was a problem in reading the map
        if( map.fail() == true )
        {
            //Stop loading map
            map.close();
            return false;
        }

        //If the number is a valid tile number
        if((tileType >= 0) && (tileType < NUM_TILE_SPRITES))
        {
            tiles[t] = new Tile(x, y, tileType);
        }
        //If we don't recognize the tile type
        else
        {
            //Stop loading map
            map.close();
            return false;
        }

        //Move to next tile spot
        x += TILE_WIDTH;

        //If we've gone too far
        if( x >= levelWidth )
        {
            //Move back
            x = 0;

            //Move to the next row
            y += TILE_HEIGHT;
        }
    }

    //Close the file
    map.close();

    //If the map was loaded fine
    return true;
}

// load tiles for editor, create map if map doesn't exist
bool loadTiles( Tile *tiles[] )
{
    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "tiling.map" );

    //If the map couldn't be loaded i.e. the map doesn't exist
    if( map == NULL )
    {
        //Initialize the tiles
        for( int t = 0; t < TOTAL_TILES; t++ )
        {
            //Put a floor tile
            tiles[ t ] = new Tile( x, y, t % ( TILE_BLUE + 1 ) );

            //Move to next tile spot
            x += TILE_WIDTH;

            //If we've gone too far
            if( x >= levelWidth )
            {
                //Move back
                x = 0;

                //Move to the next row
                y += TILE_HEIGHT;
            }
        }
    }
    else // map does exist
    {
        //Initialize the tiles
        for( int t = 0; t < TOTAL_TILES; t++ )
        {
            //Determines what kind of tile will be made
            int tileType = -1;

            //Read tile from map file
            map >> tileType;

            //If there was a problem in reading the map
            if( map.fail() == true )
            {
                //Stop loading map
                map.close();
                return false;
            }

            //If the number is a valid tile number
            if((tileType >= 0) && (tileType < NUM_TILE_SPRITES))
            {
                tiles[ t ] = new Tile( x, y, tileType );
            }
            //If we don't recognize the tile type
            else
            {
                //Stop loading map
                map.close();
                return false;
            }

            //Move to next tile spot
            x += TILE_WIDTH;

            //If we've gone too far
            if( x >= levelWidth )
            {
                //Move back
                x = 0;

                //Move to the next row
                y += TILE_HEIGHT;
            }
        }

        //Close the file
        map.close();
    }

    return true;
}

void setEditorCamera()
{
    //Mouse offsets
    int x = 0, y = 0;

    //Get mouse offsets
    SDL_GetMouseState( &x, &y );

    //Move camera to the left if needed
    if( x < TILE_WIDTH )
        camera.x -= 20;

    //Move camera to the right if needed
    if( x > screenWidth - TILE_WIDTH )
        camera.x += 20;

    //Move camera up if needed
    if( y < TILE_WIDTH )
        camera.y -= 20;

    //Move camera down if needed
    if( y > screenHeight - TILE_WIDTH )
        camera.y += 20;

    //Keep the camera in bounds.
    if(camera.x < 0)
        camera.x = 0;
    if(camera.y < 0)
        camera.y = 0;
    if(camera.x > levelWidth - camera.w)
        camera.x = levelWidth - camera.w;
    if(camera.y > levelHeight - camera.h)
        camera.y = levelHeight - camera.h;
}

void showType( int tileType )
{
    switch( tileType )
    {
        case TILE_RED:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Red Floor", NULL );
        break;

        case TILE_GREEN:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Green Floor", NULL );
        break;

        case TILE_BLUE:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Blue Floor", NULL );
        break;

        case TILE_CENTER:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Center Wall", NULL );
        break;

        case TILE_TOP:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Top Wall", NULL );
        break;

        case TILE_TOPRIGHT:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Top Right Wall", NULL );
        break;

        case TILE_RIGHT:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Right Wall", NULL );
        break;

        case TILE_BOTTOMRIGHT:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Bottom Right Wall", NULL );
        break;

        case TILE_BOTTOM:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Bottom Wall", NULL );
        break;

        case TILE_BOTTOMLEFT:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Bottom Left Wall", NULL );
        break;

        case TILE_LEFT:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Left Wall", NULL );
        break;

        case TILE_TOPLEFT:
        SDL_WM_SetCaption("S: Save, Q/W: Cycle Tiles, BACKSPACE: Start Screen. Current Tile is Top Left Wall", NULL );
        break;
    };
}

void putTile( Tile *tiles[], int tileType )
{
    //Mouse offsets
    int x = 0, y = 0;

    //Get mouse offsets
    SDL_GetMouseState( &x, &y );

    //Adjust to camera
    x += camera.x;
    y += camera.y;

    //Go through tiles
    for( int t = 0; t < TOTAL_TILES; t++ )
    {
        //Get tile's collision box
        SDL_Rect box = tiles[t]->getBox();

        //If the mouse is inside the tile
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            //Get rid of old tile
            delete tiles[t];

            //Replace it with new one
            tiles[t] = new Tile( box.x, box.y, tileType );
        }
    }
}

void saveTiles( Tile *tiles[] )
{
    //Open the map
    std::ofstream map( "tiling.map" );

    //Go through the tiles
    for( int t = 0; t < TOTAL_TILES; t++ )
    {
        //Write tile type to file
        map << tiles[t]->getType() << " ";
    }

    //Close the file
    map.close();
}

void deleteTiles(Tile *tiles[])
{
    for( int t = 0; t < TOTAL_TILES; t++ )
    {
        delete tiles[ t ];
    }
}
