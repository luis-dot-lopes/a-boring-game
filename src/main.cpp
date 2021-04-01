/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "Box.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int NUM_OF_ENEMIES = 25;

//Graphics related globals
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screenSurface = NULL;

//images
SDL_Texture *playerImg = NULL;
SDL_Texture *obstacleImg = NULL;
SDL_Texture *lostImg = NULL;
SDL_Texture *wonImg = NULL;

//Game related globals
bool quit;
bool gameover;
bool won;

bool keysPressed[242];
bool keysReleased[242];
bool keysJustPressed[242];

//Game variables
Box player(0, 0, 20, 20);
Box enemies[NUM_OF_ENEMIES];
int speeds[NUM_OF_ENEMIES];

//Functions
bool init();

//Loading Functions
bool load_media();

SDL_Surface* load_surface( std::string path );

SDL_Texture* load_texture( std::string path );

//Game Logic/Utils functions
void register_keys();

void start();

float distance(Box b1, Box b2);

bool collision_occured();

void close();

bool init() {
    quit = false;

    srand(time(NULL));

    if(SDL_Init(SDL_INIT_VIDEO < 0)) {
        printf("SDL couldn't initialize. Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("SDL Game", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        printf("Couldn't create window. Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        printf("Couldn't create renderer. Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

    screenSurface = SDL_GetWindowSurface(window);

    return true;
}

bool load_media() {

    playerImg = load_texture("player.bmp");
    if(playerImg == NULL) {
        printf("Unable to load texture %s. Error: %s\n", "player.bmp", SDL_GetError());
        return false;
    }

    obstacleImg = load_texture("obstacle.bmp");
    if(obstacleImg == NULL) {
        printf("Unable to load texture %s. Error: %s\n", "obstacle.bmp", SDL_GetError());
        return false;
    }

    lostImg = load_texture("lost.bmp");
    if(lostImg == NULL) {
        printf("Unable to load texture %s. Error: %s\n", "lost.bmp", SDL_GetError());
        return false;
    }

    wonImg = load_texture("won.bmp");
    if(wonImg == NULL) {
        printf("Unable to load texture %s. Error: %s\n", "won.bmp", SDL_GetError());
        return false;
    }

    return true;
}

SDL_Surface* load_surface(std::string path) {
    SDL_Surface* optimizedSurface = NULL;

    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if(loadedSurface == NULL) {

        printf("Unable to load image %s. Error: %s\n", path.c_str(), SDL_GetError());

    } else {

        optimizedSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, 0 );
        if(optimizedSurface == NULL) {
            printf("Unable to convert surface %s. Error: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

SDL_Texture* load_texture( std::string path ) {

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = load_surface( path.c_str() );
    if( loadedSurface == NULL ) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError() );
    } else {

        Uint32 colorKey = SDL_MapRGB(loadedSurface->format, 255, 0, 255);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, colorKey);

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL ) {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

void register_keys() {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            quit = true;
        }
    }
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    for(int i = 0; i < 242; i++) {
        if(currentKeyStates[i]) {
            keysJustPressed[i] = !keysPressed[i];
            keysPressed[i] = true;
            keysReleased[i] = false;
        } else {
            keysReleased[i] = true;
            keysPressed[i] = false;
            keysJustPressed[i] = false;
        }
    }
}

void start() {
    player.x = 10;
    player.y = 10;

    for(int i = 0; i < NUM_OF_ENEMIES; i++) {
        int x = rand() % 450 + 100;
        int y = rand() % 460;
        Box newEnemy(x, y, 20, 20);

        enemies[i] = newEnemy;

        speeds[i] = rand() % 7 - 3;
    }

    gameover = false;
    won = false;
}

float distance(Box b1, Box b2) {
    float centerX1 = b1.x + 10;
    float centerY1 = b1.y + 10;
    float centerX2 = b2.x + 10;
    float centerY2 = b2.y + 10;

    return sqrt((centerX1 - centerX2) * (centerX1 - centerX2)
              + (centerY1 - centerY2) * (centerY1 - centerY2));
}

bool collision_occured() {
    for(int i = 0; i < NUM_OF_ENEMIES; i++) {
        if(player.has_collided(&enemies[i]) && distance(player, enemies[i]) < 18) {
            return true;
        }
    }
    return false;
}

void close() {

    SDL_DestroyTexture(playerImg);

    playerImg = NULL;

    SDL_DestroyTexture(obstacleImg);

    obstacleImg = NULL;

    SDL_DestroyTexture(lostImg);

    lostImg = NULL;

    SDL_DestroyTexture(wonImg);

    wonImg = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    window = NULL;
    renderer = NULL;

    SDL_Quit();

}

int main( int argc, char* args[] ) {
	//Initialize SDL
	if(!init()) {
        printf("Unable to initialize.");
        return -1;
	}
	if(!load_media()) {
        printf("Unable to load media.");
        return -1;
	}

	start();

	while(!quit) {

        register_keys();

        if(gameover) {

            if(keysPressed[SDL_SCANCODE_SPACE])
                start();

            SDL_RenderClear(renderer);

            if(won) {

                SDL_Rect screenRect = {0, 0, 640, 480};

                SDL_RenderCopy(renderer, wonImg, NULL, &screenRect);

            } else {

                SDL_Rect screenRect = {0, 0, 640, 480};

                SDL_RenderCopy(renderer, lostImg, NULL, &screenRect);

            }

            SDL_RenderPresent(renderer);

            SDL_Delay(10);

            continue;
        }

        if(keysPressed[SDL_SCANCODE_UP]) {
            player.y -= 3;
            if(collision_occured()) {
                gameover = true;
                continue;
            }
        }

        if(keysPressed[SDL_SCANCODE_DOWN]) {
            player.y += 3;
            if(collision_occured()) {
                gameover = true;
                continue;
            }
        }

        if(keysPressed[SDL_SCANCODE_LEFT]) {
            player.x -= 3;
            if(collision_occured()) {
                gameover = true;
                continue;
            }
        }

        if(keysPressed[SDL_SCANCODE_RIGHT]) {
            player.x += 3;
            if(collision_occured()) {
                gameover = true;
                continue;
            }
        }

        if(player.x > SCREEN_WIDTH) {
            won = true;
            gameover = true;
            continue;
        }

        for(int i = 0; i < NUM_OF_ENEMIES; i++) {
            enemies[i].y += speeds[i];
            if(enemies[i].y < 0 || enemies[i].y + 20 > SCREEN_HEIGHT) {
                speeds[i] *= -1;
            }
        }

        SDL_RenderClear( renderer );

        SDL_Rect posRect = player.as_SDL_rect();

        SDL_RenderCopy(renderer, playerImg, NULL, &posRect);

        for(int i = 0; i < NUM_OF_ENEMIES; i++) {
            SDL_Rect pos2Rect = enemies[i].as_SDL_rect();

            SDL_RenderCopy(renderer, obstacleImg, NULL, &pos2Rect);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(10);

	}

    close();

	return 0;
}
