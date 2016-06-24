#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "SDL_Media.h"
#include "bool.h"

//Janela e Render:
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
//Fonte TTF para textos:
TTF_Font* fonte = NULL;

//initialize sdl:
bool init() {
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
                                    SCREEN_WIDTH,
                                    SCREEN_HEIGHT,
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
                // inicia sdl image
                int flag = IMG_INIT_PNG | IMG_INIT_JPG;
                if (!( IMG_Init(flag) & flag )) {
                    printf( "Erro ao iniciar sdl_image! Erro: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }
    return success;
}

// carrega uma imagem e retorna a textura criada
SDL_Texture* carregarImagem (char *arquivo) {
    // textura que será retornada
    SDL_Texture *resultado = NULL;

    // imagem carregada a partir do arquivo
    SDL_Surface *carregada = IMG_Load(arquivo);

    // verifica se não é null
    if (carregada == NULL) {
        printf("Nao foi possivel carregar a imagem %s, erro: %s", arquivo, IMG_GetError());
    } else {
        // cria a otimizada, que sera mais rapida para o renderer varias vezes
        resultado = SDL_CreateTextureFromSurface(gRenderer, carregada);

        // verifica
        if (resultado == NULL) {
            printf("Erro ao otimizar a imagem %s, erro: %s", arquivo, SDL_GetError());
        }

        // libera a carregada
        SDL_FreeSurface(carregada);
    }
    // retorna o resultado
    return resultado;
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
    IMG_Quit();
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

//gera a textura com o tempo recebido e corrige a posicao: (cesar)
SDL_Texture* criarTexture(int tempo, SDL_Color cor, SDL_Rect* rect) {
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
        rect->x = (SCREEN_WIDTH - rect->w)/2;
        // libera a surface
        SDL_FreeSurface(textSurface);
        // retorna
        return textTexture;
    }
    return NULL;
}

//atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
SDL_Texture* atualizaTempoMostrado(int* tempoRestante, SDL_Texture* tempoTexture, int* contador, SDL_Color *tempoColor, SDL_Rect *tempoRect){
    (*tempoRestante)--;
    SDL_DestroyTexture(tempoTexture);
    *contador = SDL_GetTicks();

    return criarTexture(*tempoRestante, *tempoColor, tempoRect);
}
