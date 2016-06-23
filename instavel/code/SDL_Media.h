//esse arquivo contem as principais funções de inicialização, carregamento e encerramento do SDL

#ifndef SDL_MAIN_H
    #define SDL_MAIN_H

    // dimensões da tela
    #define SCREEN_WIDTH 1000
    #define SCREEN_HEIGHT 700

    #include <SDL.h>
    #include "bool.h"

    extern SDL_Window* gWindow;
    extern SDL_Renderer* gRenderer;

    //initialize sdl:
    bool init();
    //permite que o usuário fecha a janela clicando no X: (flavio)
    void closeWindow(SDL_Event* event, bool* running);
    //close sdl
    void quit();// carrega uma imagem e retorna a textura criada (cesar)
    SDL_Texture* carregarImagem (SDL_Renderer* renderer, char *arquivo);
    //gera a textura com o tempo recebido e corrige a posicao: (cesar)
    SDL_Texture* criarTexture(int tempo, SDL_Color cor, SDL_Rect* rect);
    //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
    SDL_Texture* atualizaTempoMostrado(int* tempoRestante, SDL_Texture* tempoTexture, int* contador, SDL_Color *tempoColor, SDL_Rect *tempoRect);
    //carrega e toca a musica:
    void play_Music(char path[]);
#endif
