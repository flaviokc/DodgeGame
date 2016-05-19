#include <SDL.h>
#include <SDL_mixer.h>
#include "./code/SDL_Main.h"
#include "./code/SDL_Physics.h"
#include "./code/SDL_Fases.h"
#include "./code/bool.h"

//https://github.com/flaviokc/DodgeGame

//modos:
void historia(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT);

//carrega e toca a musica:
void play_Music(char path[]);


//menu
int main (int argc, char* args[]) {
    //dimensões da tela:
    const int SCREEN_WIDTH = 1000;
    const int SCREEN_HEIGHT = 700;

    //Start up SDL and create window
    if( !init(&SCREEN_WIDTH, &SCREEN_HEIGHT) ) {
        printf( "Failed to initialize!\n" );
    }
    else {
        play_Music("./media/songs/musica1.ogg"); //carrega e toca a musica:

        //coloquem cada modo dentro de uma função pra dps fazer o menu:
        historia(&SCREEN_WIDTH, &SCREEN_HEIGHT);
    }

    quit();
    return 0;
}

//menu do modo historia:
void historia(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT){
    fase1(SCREEN_WIDTH, SCREEN_HEIGHT);
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

