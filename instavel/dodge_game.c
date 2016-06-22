#include <SDL.h>
#include <SDL_mixer.h>
#include "./code/SDL_Main.h"
#include "./code/SDL_Physics.h"
#include "./code/SDL_Fases.h"
#include "./code/bool.h"

//https://github.com/flaviokc/DodgeGame

//modos:
void historia();

//menu
int main (int argc, char* args[]) {

    //Start up SDL and create window
    if( !init() ) {
        printf( "Failed to initialize!\n" );
    }
    else {
        play_Music("./media/songs/musica1.ogg"); //carrega e toca a musica:

        //coloquem cada modo dentro de uma função pra dps fazer o menu:
        historia();
    }

    quit();
    return 0;
}

//menu do modo historia:
void historia(){
    //fasebug();
    //faseExemplo();
    //faseTutorial3();
    faseTematica1();
}
