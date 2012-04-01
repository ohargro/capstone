/*--------------------------------
  Author:  Chris Dempewolf, Bryan Glazer, Orren Hargr
  Date:    Tuesday, January 31 2012
  File:    base.cpp
  Purpose:
---------------------------------*/
#include "base.h"
#include "window.h"
#include "timer.h"
#include "sprite.h"
#include "enemy.h"

//global variables & constant(s)
SDL_Surface* screen = NULL;
SDL_Surface* background = NULL;
SDL_Surface* startBackground = NULL;
SDL_Surface* tileSheet = NULL;
SDL_Surface* enemySheet = NULL;
SDL_Event event;
int screenWidth = 640;
int screenHeight = 380;
int levelWidth = 1280;
int levelHeight = 960;
SDL_Rect camera = {0, 0, screenWidth, screenHeight};
int screenBPP = 32;
Mix_Music* music = NULL;
const int FRAMES_PER_SECOND = 20;

const int GRAVITY = 150;
const int RIGHT = 2;
const int LEFT = 1;

bool load_files()
{
    background = load_image("bg.png");
    startBackground = load_image("gear.png");
    tileSheet = load_image("tiles.png");
    enemySheet = load_image("enemySheet.png", 32, 156, 0);
    music = Mix_LoadMUS("beat.wav");

    if (background == NULL)
        return false;
    if (startBackground == NULL)
        return false;
    if (tileSheet == NULL)
        return false;
    if (enemySheet == NULL)
        return false;
    if (music == NULL)
        return false;

    return true;
}

bool init(int screenWidth, int screenHeight, int screenBPP)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return false;
    if(TTF_Init() == -1)
        return false;
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        return false;
    screen = SDL_SetVideoMode(screenWidth, screenHeight, screenBPP, SDL_DOUBLEBUF | SDL_HWSURFACE);
    if(screen == NULL)
        return false;
    SDL_WM_SetCaption("9: Pause/Unpause Music, 0: Stop Music, ENTER: Fullscreen", NULL);
    return true;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* dest)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, NULL, dest, &offset);
    SDL_Flip(screen);
}

//Apply source onto dest at {x, y} relative(?) to camera
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect* camera)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, camera, dest, &offset);
    SDL_Flip(screen);
}

SDL_Surface* load_image(std::string filename)
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load(filename.c_str());
    if(loadedImage != NULL)
    {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);

        if(optimizedImage != NULL)
        {
            SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
        }
    }
    return optimizedImage;

}

//Load image and set pixels that are equal to RGB to be transparent
SDL_Surface* load_image(std::string filename, int red, int green, int blue)
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load(filename.c_str());
    if(loadedImage != NULL)
    {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);

        if(optimizedImage != NULL)
        {
            //Map the color key
            Uint32 colorKey = SDL_MapRGB(optimizedImage->format, red, green, blue);

            SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorKey);
        }
    }
    return optimizedImage;

}

SDL_Surface* load_font(std::string fontName, std::string text, SDL_Color color, int size)
{
    TTF_Font* font = TTF_OpenFont(fontName.c_str(), size);
    SDL_Surface* temp = TTF_RenderText_Solid(font, text.c_str(), color);
    TTF_CloseFont(font);
    return temp;
}

bool checkCollision( SDL_Rect &a, SDL_Rect &b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if ((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB))
        return false;

    //If none of the sides from A are outside B
    return true;
}

void handleMusic()
{
    if (event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_9:
            {
                if( Mix_PlayingMusic() == 0 ) //No music playing
                {
                    if( Mix_PlayMusic( music, -1 ) == -1 )
                        break;;
                }
                //If music is being played
                else
                {
                    if( Mix_PausedMusic() == 1 )
                        Mix_ResumeMusic();
                    //If the music is playing
                    else
                        Mix_PauseMusic();
                }
            }
            break;
            case SDLK_0: Mix_HaltMusic(); break;
            default:;
        }
    }
}

void cleanUp()
{
    SDL_FreeSurface(screen);
    SDL_FreeSurface(startBackground);
    SDL_FreeSurface(tileSheet);
    SDL_FreeSurface(enemySheet);
    SDL_FreeSurface(background);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

void startScreen()
{
    SDL_WM_SetCaption("9: Pause/Unpause Music, 0: Stop Music, ENTER: Fullscreen", NULL);
    SDL_Color startFontColor = {175, 175, 175};
    SDL_Surface* cyborgMessage = NULL;
    SDL_Surface* startMessage = NULL;
    SDL_Surface* editMessage = NULL;
    cyborgMessage = load_font("eva.ttf", "CYBORG", startFontColor, 79);
    startMessage = load_font("eva.ttf", "BEGIN", startFontColor, 48);
    editMessage = load_font("eva.ttf", "EDIT", startFontColor, 24);

    while(true)
    {
        fps.start();

        while(SDL_PollEvent(&event))
        {
            int x = 0, y = 0;
            if (event.type == SDL_MOUSEMOTION) // mouse moved
            {
                // mouse offsets
                x = event.motion.x;
                y = event.motion.y;

                //If mouse is over startMessage
                if ((x > (screenWidth - 270)) && (x < (screenWidth - 270) + startMessage->w) && (y > (screenHeight - 60)) && (y < (screenHeight - 60) + startMessage->h))
                {
                    //Make startMessage turn white
                    startMessage = load_font("eva.ttf","BEGIN", {255, 255, 255}, 48);
                }
                else
                {
                    //Make startMessage turn grey
                    startMessage = load_font("eva.ttf","BEGIN", {175, 175, 175}, 48);
                }

                // if mouse is over editMessage
                if ((x > (screenWidth - 100)) && (x < (screenWidth - 100) + editMessage->w) && (y > (screenHeight - 92)) && (y < (screenHeight - 92) + editMessage->h))
                {
                    //Make editMessage turn white
                    editMessage = load_font("eva.ttf","EDIT", {255, 255, 255}, 24);
                }
                else
                {
                    //Make editMessage turn grey
                    editMessage = load_font("eva.ttf","EDIT", {175, 175, 175}, 24);
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                x = event.motion.x;
                y = event.motion.y;
                //If mouse has clicked on startMessage
                if ((x > (screenWidth - 270)) && (x < (screenWidth - 270) + startMessage->w) && (y > (screenHeight - 60)) && (y < (screenHeight - 60) + startMessage->h))
                {
                    fps.stop();
                    SDL_FreeSurface(cyborgMessage);
                    SDL_FreeSurface(startMessage);
                    SDL_FreeSurface(editMessage);
                    theGame();
                }

                //If mouse has clicked on editMessage
                if ((x > (screenWidth - 100)) && (x < (screenWidth - 100) + editMessage->w) && (y > (screenHeight - 92)) && (y < (screenHeight - 92) + editMessage->h))
                {
                    fps.stop();
                    SDL_FreeSurface(cyborgMessage);
                    SDL_FreeSurface(startMessage);
                    SDL_FreeSurface(editMessage);
                    editLevelScreen();
                }
            }

            myWindow.handle_window();
            handleMusic();

            if(event.type == SDL_QUIT)
            {
                SDL_FreeSurface(cyborgMessage);
                SDL_FreeSurface(startMessage);
                SDL_FreeSurface(editMessage);
                cleanUp();
                exit(0);
            }
        }
        if (myWindow.error())
        {
            cleanUp();
            exit(0);
        }

        apply_surface(0, 0, startBackground, screen);
        apply_surface(screenWidth - cyborgMessage->w - 1 , 5, cyborgMessage, startBackground);
        apply_surface(screenWidth - startMessage->w - 1, screenHeight - startMessage->h - 1, startMessage, startBackground);
        apply_surface(screenWidth - editMessage->w - 1, screenHeight - startMessage->h - editMessage->h - 1, editMessage, startBackground);

        if(SDL_Flip(screen) == -1)
        {
            cleanUp();
            exit(0);
        }

        if(fps.get_ticks() < (1000 / FRAMES_PER_SECOND))
            SDL_Delay(1000 / FRAMES_PER_SECOND - fps.get_ticks());
    }
}

void theGame()
{
    SDL_WM_SetCaption("WASD: Move, BACKSPACE: Start Screen, ENTER: Fullscreen", NULL);
    Sprite mew("testChar2.png", 32, 156, 0); //load mew and set R 32, G 156, B 0 to be transparent.  Set width to 33 and height to 69

    Tile *tiles[TOTAL_TILES];
    Enemy *enemies[TOTAL_ENEMIES];

    enemies[0] = new Enemy(225, 0, ENEMY_ROBOT);
    enemies[0]->setVelX(50);

    enemies[1] = new Enemy(225, 0, ENEMY_ROBOT);
    enemies[1]->setVelX(100);

    enemies[2] = new Enemy(225, 0, ENEMY_ANDROID);
    enemies[2]->setVelX(-75);

    bool playing = true;

    if(setTiles(tiles) == false)
    {
        cleanUp();
        exit(0);
    }

    //game loop
    delta.start();
    while(playing)
    {
        fps.start();
        while(SDL_PollEvent(&event))
        {
            mew.handle_input();
            myWindow.handle_window();
            handleMusic();

            if(event.key.keysym.sym == SDLK_BACKSPACE)
            {
                deleteTiles(tiles);
                deleteEnemies(enemies);
                startScreen();
            }
            if(event.type == SDL_QUIT)
                playing = false;
        }
        if (myWindow.error())
        {
            cleanUp();
            exit(0);
        }

        mew.move(delta.get_ticks(), tiles, enemies);

        for (int e = 0; e < TOTAL_ENEMIES; e++)
        {
            enemies[e]->walk(delta.get_ticks(), tiles);
        }

        delta.start(); //restart delta after movement
        mew.setCamera();

        for( int t = 0; t < TOTAL_TILES; t++ )
            tiles[t]->show();
        mew.show();
        for (int e = 0; e < TOTAL_ENEMIES; e++)
            enemies[e]->show();

        if(SDL_Flip(screen) == -1)
        {
            cleanUp();
            exit(0);
        }

        if(fps.get_ticks() < (1000 / FRAMES_PER_SECOND))
            SDL_Delay(1000 / FRAMES_PER_SECOND - fps.get_ticks());

    }

    deleteTiles(tiles);
    deleteEnemies(enemies);
    exit(0);
}

void editLevelScreen()
{
    int currentType = TILE_RED;
    showType(currentType);

    Tile *tiles[TOTAL_TILES];

    if(loadTiles(tiles) == false)
    {
        cleanUp();
        exit(0);
    }

    while(true)
    {
        fps.start();

        while(SDL_PollEvent(&event))
        {
            //When the user clicks
            if( event.type == SDL_MOUSEBUTTONDOWN )
            {
                //Put the tile
                putTile( tiles, currentType );
            }

            if (event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_1:
                    case SDLK_q:
                    {
                        //Scroll forward through tiles
                        currentType--;

                        if( currentType < TILE_RED )
                            currentType = TILE_TOPLEFT;

                        //Show the current tile type
                        showType( currentType );
                        break;
                    }
                    case SDLK_2:
                    case SDLK_w:
                    {
                        //Scroll backward through tiles
                        currentType++;

                        if( currentType > TILE_TOPLEFT )
                            currentType = TILE_RED;

                        //Show the current tile type
                        showType( currentType );
                        break;
                    }

                    case SDLK_s:
                    {
                        // save the tiles and display a "saved" message
                        saveTiles(tiles);
                        SDL_Color savedMessageColor = {255, 255, 255};
                        SDL_Surface* savedMessage = NULL;
                        savedMessage = load_font("eva.ttf", "SAVED", savedMessageColor, 24);
                        apply_surface(1, 1, savedMessage, screen);
                        SDL_Delay(500);
                        SDL_FreeSurface(savedMessage);
                        break;
                    }
                    case SDLK_BACKSPACE:
                    {
                        deleteTiles(tiles);
                        startScreen();
                        break;
                    }
                    default:;
                }
            }

            myWindow.handle_window();
            handleMusic();

            if( event.type == SDL_QUIT )
            {
                //Quit the program
                deleteTiles(tiles);
                cleanUp();
                exit(0);
            }
        }

        if (myWindow.error())
        {
            cleanUp();
            exit(0);
        }

        setEditorCamera();

        for (int t = 0; t < TOTAL_TILES; t++)
            tiles[t]->show();

        if (SDL_Flip(screen) == -1)
        {
            cleanUp();
            exit(0);
        }

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }
}
