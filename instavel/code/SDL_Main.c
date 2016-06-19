#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "SDL_Main.h"
#include "bool.h"

//Janela e Render:
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//Fonte TTF para textos:
TTF_Font* fonte = NULL;

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
            // inicia o TTF para textos
            if (TTF_Init() == -1) {
                printf("Erro ao iniciar TTF, erro: %s", TTF_GetError());
                success = false;
            } else {
                // carrega a fonte
                // "./media/fonts/arial.ttf"
                fonte = TTF_OpenFont("./media/fonts/arial.ttf", 60);
                if (fonte == NULL) {
                    printf("erro ao carregar a fonte, erro: %s", TTF_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

//gera a textura com o tempo recebido e corrige a posicao: (cesar)
SDL_Texture* criarTexture(int tempo, SDL_Color cor, SDL_Rect* rect, const int* SCREEN_WIDTH) {
    char texto[2];
    sprintf(texto, "%d", tempo);
    // carrega uma surface com a cor e o texto
    SDL_Surface* textSurface = TTF_RenderText_Solid(fonte, texto, cor);

    if (textSurface == NULL) {
        printf("erro ao criar texto, erro: %s", TTF_GetError());
    } else {
        // transforma a surface numa texture
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        // salva w e h
        rect->w = textSurface->w;
        rect->h = textSurface->h;
        //calcula e salva x
        rect->x = (*SCREEN_WIDTH - rect->w)/2;
        // libera a surface
        SDL_FreeSurface(textSurface);
        // retorna
        return textTexture;
    }
    return NULL;
}

//permite que o usuário fecha a janela clicando no X: (flavio)
void closeWindow(SDL_Event* event, bool* running){
    if(event->type == SDL_QUIT){
        *running = false;
    }
}

//close sdl:
void quit() {
    // encerra a fonte
    TTF_CloseFont(fonte);
    fonte = NULL;
    // encerra window e renderer
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    // para a musica
    Mix_HaltMusic();
    // sai de todas bibliotecas
    TTF_Quit();
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

//atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
SDL_Texture* atualizaTempoMostrado(int* tempoRestante, SDL_Texture* tempoTexture, int* contador, SDL_Color *tempoColor, SDL_Rect *tempoRect, const int* SCREEN_WIDTH){
    (*tempoRestante)--;
    SDL_DestroyTexture(tempoTexture);
    *contador = SDL_GetTicks();

    return criarTexture(*tempoRestante, *tempoColor, tempoRect, SCREEN_WIDTH);
}
