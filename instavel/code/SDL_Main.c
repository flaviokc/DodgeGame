#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include "SDL_Main.h"
#include "bool.h"

//Janela e Render:
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//initialize sdl:
bool init(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT) {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else {
        //Create window
        gWindow = SDL_CreateWindow( "Dodge Game",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    *SCREEN_WIDTH,
                                    *SCREEN_HEIGHT,
                                    SDL_WINDOW_RESIZABLE);

        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else {
            //set the renderer:
            gRenderer=SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL){
                printf( "Renderer could not be set! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            // inicia o mixer para sons
            if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
                printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            }
        }
    }
    return success;
}

//permite que o usuário fecha a janela clicando no X: (flavio)
void closeWindow(SDL_Event* event, bool* running){
    if(event->type == SDL_QUIT){
        *running = false;
    }
}

//close sdl:
void quit() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    Mix_HaltMusic();
    Mix_Quit();
    SDL_Quit();
}

//carrega e toca a musica: (passar para o main)
void play_Music(char path[]){
    Mix_Music* musica = NULL;
    musica = Mix_LoadMUS(path);
    if (musica == NULL) {
        printf("Erro ao ler a musica, erro: %s", Mix_GetError());
    } else {
        Mix_PlayMusic(musica, -1);
    }
}