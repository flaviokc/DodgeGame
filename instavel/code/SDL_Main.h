//esse arquivo contem as principais funções de inicialização, carregamento e encerramento do SDL

#ifndef SDL_MAIN_H
    #define SDL_MAIN_H

    #include <SDL.h>
    //#include <SDL_mixer.h>
    #include "bool.h"

    extern SDL_Window* gWindow;
    extern SDL_Renderer* gRenderer;

    //initialize sdl:
    bool init(const int *SCREEN_WIDTH, const int *SCREEN_HEIGHT);
    //permite que o usuário fecha a janela clicando no X: (flavio)
    void closeWindow(SDL_Event* event, bool* running);
    //close sdl
    void quit();
    //gera a textura com o tempo recebido e corrige a posicao: (cesar)
    SDL_Texture* criarTexture(int tempo, SDL_Color cor, SDL_Rect* rect, const int* SCREEN_WIDTH);
    //carrega e toca a musica:
    void play_Music(char path[]);
#endif
