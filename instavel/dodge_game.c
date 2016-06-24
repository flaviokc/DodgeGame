#include <SDL.h>
#include <SDL_mixer.h>
#include "./code/SDL_Media.h"
#include "./code/SDL_Physics.h"
#include "./code/SDL_Fases.h"
#include "./code/bool.h"

//https://github.com/flaviokc/DodgeGame

//modos:
void historia();
void menuInicial();

//menu
int main (int argc, char* args[]) {

    //Start up SDL and create window
    if( !init() ) {
        printf( "Failed to initialize!\n" );
    }
    else {
        play_Music("./media/songs/musica1.ogg"); //carrega e toca a musica:

        //coloquem cada modo dentro de uma função pra dps fazer o menu:
        menuInicial();
    }

    quit();
    return 0;
}

bool collRectPoint (SDL_Rect* rect, int x, int y) {
    return ( x > rect->x && x < rect->x+rect->w && y > rect->y && y < rect->y+rect->h );
}

// verifica se um rect está com o mouse em cima e altera seu tamanho quando necessário (cesar)
void hoverRect (SDL_Rect* rect, bool* hover, int x, int y, int varX, int varY) {
    if (collRectPoint(rect, x, y)) {
        if (!*hover) {
            rect->w += varX;
            rect->h += varY;
            rect->x -= varX/2;
            rect->y -= varY/2;
            *hover = true;
        }
    } else if (*hover) {
            rect->w -= varX;
            rect->h -= varY;
            rect->x += varX/2;
            rect->y += varY/2;
            *hover = false;
    }
}

void transicao() {
    SDL_Rect tudo = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    int alfa = 0;
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(gRenderer, 255, 222, 129, 50);

    while (alfa < 24) {
        SDL_SetRenderDrawColor(gRenderer, 255, 222, 129, 10);
        SDL_RenderFillRect(gRenderer, &tudo);
        SDL_RenderPresent(gRenderer);
        alfa++;
        SDL_Delay(20);
    }
}

// menu principal do jogo (cesar)
void menuInicial() {
    // VARIAVEIS
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    int contWhile = 1; //sempre contWhile = 1;
    bool running = true;
    int x, y;

    // ANIMAÇAO DO TITULO
    float angulo = -8;
    float variacaoAngulo = 0.30;

    // TEXTURAS
    SDL_Texture* tituloTexture = carregarImagem("./media/menu/m1.png");
    SDL_Texture* bTutorialTexture = carregarImagem("./media/menu/m9.png");
    SDL_Texture* bHistoriaTexture = carregarImagem("./media/menu/m2.png");
    SDL_Texture* bMultiTexture = carregarImagem("./media/menu/m3.png");
    SDL_Texture* personagem1Texture = carregarImagem("./media/skins/player/apaixonado.png");
    SDL_Texture* personagem2Texture = carregarImagem("./media/skins/player/papai noel.png");

    // RECTS
    SDL_Rect tituloRect = {150, 50, 700, 125};
    SDL_Rect bTutorialRect = {340, 225, 320, 85};
    SDL_Rect bHistoriaRect = {250, 360, 500, 85};
    SDL_Rect bMultiRect = {180, 495, 640, 82};

    // PERSONAGENS
    SDL_Rect personagem1 = {500, 350, 100, 100};
    Var varPersonagem1 = {4,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    SDL_Rect personagem2 = {200, 200, 100, 100};
    Var varPersonagem2 = {-3,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)

    // BOOLS
    bool bTutorialHover = false;
    bool bHistoriaHover = false;
    bool bMultiHover = false;

    while(running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            // clique do mouse
            if (event->type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&x, &y);
                if (collRectPoint(&bHistoriaRect, x, y)) {
                    transicao();
                    historia();
                }
            }
            // movimento do mouse
            if (event->type == SDL_MOUSEMOTION) {
                SDL_GetMouseState(&x, &y);
                hoverRect(&bHistoriaRect, &bHistoriaHover, x, y, 60, 12);
                hoverRect(&bTutorialRect, &bTutorialHover, x, y, 60, 12);
                hoverRect(&bMultiRect, &bMultiHover, x, y, 60, 12);
            }
        }

        // atualiza o angulo
        angulo += variacaoAngulo;
        if (angulo > 8 || angulo < -8) {
            variacaoAngulo *= -1;
        }

        //INTERACOES FISICAS
        //personagem1
        collRectWall(&personagem1, &varPersonagem1);
        acelObj(&varPersonagem1, contWhile);
        velObj(&personagem1.x, &personagem1.y, &varPersonagem1.vX, &varPersonagem1.vY);

        //personagem2
        collRectWall(&personagem2, &varPersonagem2);
        acelObj(&varPersonagem2, contWhile);
        velObj(&personagem2.x, &personagem2.y, &varPersonagem2.vX, &varPersonagem2.vY);

        // pinta o fundo
        SDL_SetRenderDrawColor(gRenderer, 255, 222, 129, 255);
        SDL_RenderClear(gRenderer);

        // imprime os persongens
        SDL_RenderCopy(gRenderer, personagem1Texture, 0, &personagem1);
        SDL_RenderCopy(gRenderer, personagem2Texture, 0, &personagem2);

        // imprime os botões
        SDL_RenderCopyEx(gRenderer, tituloTexture, 0, &tituloRect, angulo, 0, 0);
        SDL_RenderCopy(gRenderer, bTutorialTexture, 0, &bTutorialRect);
        SDL_RenderCopy(gRenderer, bHistoriaTexture, 0, &bHistoriaRect);
        SDL_RenderCopy(gRenderer, bMultiTexture, 0, &bMultiRect);

        SDL_RenderPresent(gRenderer);

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 4); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)

        SDL_Delay(20);
    }
    // elimina as texturas
    SDL_DestroyTexture(bHistoriaTexture);
    SDL_DestroyTexture(bTutorialTexture);
    SDL_DestroyTexture(bMultiTexture);
    SDL_DestroyTexture(personagem1Texture);
    SDL_DestroyTexture(personagem2Texture);
    free(event);
}

//menu do modo historia:
void historia(){

    // variaveis
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true;

    // cria vetores de texture e rect para todas imagens
    SDL_Texture* textures[9];
    SDL_Rect rects[9];
    bool hover[9];

    int i;
    char nome[30];
    for (i = 0; i < 9; i++) {
        sprintf(nome, "./media/thumbnails/fase%d.png", i+1);
        textures[i] = carregarImagem(nome);
        SDL_SetTextureBlendMode(textures[i], SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(textures[i], 0);
        rects[i].h = 200;
        rects[i].w = 300;
        rects[i].x = (((i%3) + 1) * 25) + ((i%3) * 300);
        rects[i].y = (((i/3) + 1) * 25) + ((i/3) * 200);
        hover[i] = false;
    }

    int x, y;
    int animacao = 0;
    while(running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            // clique do mouse
            if (event->type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&x, &y);
                if (collRectPoint(&rects[0], x, y)) {
                    transicao();
                    faseTematica8();
                }
            }
            // movimento do mouse
            if (event->type == SDL_MOUSEMOTION) {
                SDL_GetMouseState(&x, &y);
                for (i = 0; i < 9; i++) {
                    hoverRect(&rects[i], &hover[i], x, y, 20, 20);
                }
            }
        }

        if (animacao < 10*9) {
            SDL_SetTextureAlphaMod(textures[animacao/10], (animacao%10*255)/9);
            animacao++;
        }

        // pinta o fundo
        SDL_SetRenderDrawColor(gRenderer, 255, 222, 129, 255);
        SDL_RenderClear(gRenderer);

        // imprime todas imagens
        for (i = 0; i < 9; i++) {
            SDL_RenderCopy(gRenderer, textures[i], 0, &rects[i]);
        }

        SDL_RenderPresent(gRenderer);

        SDL_Delay(20);
    }
    transicao();
    //fasebug();
    //faseExemplo();
    //faseTutorial3();
    //faseTematica3();
    free(event);
}
