#ifndef SDL_FASES_H
    #define SDL_FASES_H

    #include <SDL.h>
    #include "bool.h"

    //esse arquivo declara as fases do jogo.

    //fases:
    void faseExemplo(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT);
    void fasebug(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT);

    //Tutorial:
    void faseTutorial1(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT);
    void faseTutorial2(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT);
    void faseTutorial3(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT);
    //Tematicas:
    void faseTematica1(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT);

#endif
