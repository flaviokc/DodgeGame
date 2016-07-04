#include <SDL.h>
#include <SDL_mixer.h>
#include "./code/SDL_Media.h"
#include "./code/SDL_Physics.h"
#include "./code/SDL_Fases.h"
#include "./code/bool.h"

#define FASES 8

//https://github.com/flaviokc/DodgeGame

//modos:
bool historia();
void tutorial(float* angulo);
void menuInicial();
void ganhou(int fase);

//menu
int main (int argc, char* args[]) {

    //Start up SDL and create window
    if( !init() ) {
        printf( "Failed to initialize!\n" );
    } else {
        play_Music("./media/songs/All_About_That_Bass.ogg"); //carrega e toca a musica:

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

void transicao () {
    SDL_Rect tudo = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    int alfa = 0;
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(gRenderer, 255, 222, 129, 50);

    while (alfa < 20) {
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

    while(running) {
        //verifica os eventos:
        while(SDL_PollEvent(event)) {
            //fechar janela
            closeWindow(event, &running);
            // clique do mouse
            if (event->type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&x, &y);
                if (collRectPoint(&bHistoriaRect, x, y)) {
                    transicao();
                    while (historia());
                    transicao();
                } else if (collRectPoint(&bTutorialRect, x, y)) {
                    transicao();
                    tutorial(&angulo);
                    transicao();
                } else if (collRectPoint(&bMultiRect, x, y)) {
                    transicao();
                    int resultado = faseMulti();
                    if (resultado > 0) {
                        transicao();
                        ganhou(resultado);
                    }
                    transicao();
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

void tutorial(float* angulo) {
    // VARIAVEIS
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true;
    int x, y;

    // ANIMAÇAO DO TITULO
    float variacaoAngulo = 0.30;

    // TEXTURAS
    SDL_Texture* tituloTexture = carregarImagem("./media/menu/m1.png");
    SDL_Texture* tuto1Texture = carregarImagem("./media/menu/tuto1.png");
    SDL_Texture* tuto2Texture = carregarImagem("./media/menu/tuto2.png");
    SDL_Texture* tuto3Texture = carregarImagem("./media/menu/tuto3.png");

    // RECTS
    SDL_Rect tituloRect = {150, 50, 700, 125};
    SDL_Rect tuto1Rect = {200, 225, 600, 120};
    SDL_Rect tuto2Rect = {200, 370, 600, 120};
    SDL_Rect tuto3Rect = {200, 515, 600, 120};

    // BOOLS
    bool tuto1Hover = false;
    bool tuto2Hover = false;
    bool tuto3Hover = false;

    while(running) {
        //verifica os eventos:
        while(SDL_PollEvent(event)) {
            //fechar janela
            closeWindow(event, &running);
            // clique do mouse
            if (event->type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&x, &y);
                if (collRectPoint(&tuto1Rect, x, y)) {
                    transicao();
                    faseTutorial1();
                    transicao();
                } else if (collRectPoint(&tuto2Rect, x, y)) {
                    transicao();
                    faseTutorial2();
                    transicao();
                } else if (collRectPoint(&tuto3Rect, x, y)) {
                    transicao();
                    faseTutorial3();
                    transicao();
                }
            }
            // movimento do mouse
            if (event->type == SDL_MOUSEMOTION) {
                SDL_GetMouseState(&x, &y);
                hoverRect(&tuto1Rect, &tuto1Hover, x, y, 60, 12);
                hoverRect(&tuto2Rect, &tuto2Hover, x, y, 60, 12);
                hoverRect(&tuto3Rect, &tuto3Hover, x, y, 60, 12);
            }
        }

        // atualiza o angulo
        *angulo += variacaoAngulo;
        if (*angulo > 8 || *angulo < -8) {
            variacaoAngulo *= -1;
        }

        // pinta o fundo
        SDL_SetRenderDrawColor(gRenderer, 255, 222, 129, 255);
        SDL_RenderClear(gRenderer);

        // imprime os botões
        SDL_RenderCopyEx(gRenderer, tituloTexture, 0, &tituloRect, *angulo, 0, 0);
        SDL_RenderCopy(gRenderer, tuto1Texture, 0, &tuto1Rect);
        SDL_RenderCopy(gRenderer, tuto2Texture, 0, &tuto2Rect);
        SDL_RenderCopy(gRenderer, tuto3Texture, 0, &tuto3Rect);

        SDL_RenderPresent(gRenderer);

        SDL_Delay(20);
    }
    // elimina as texturas
    SDL_DestroyTexture(tuto1Texture);
    SDL_DestroyTexture(tuto2Texture);
    SDL_DestroyTexture(tuto3Texture);
    free(event);
}

int faseNumero (int i) {
    switch (i) {
    case 0:
        return faseTematica1();
    case 1:
        return faseTematica2();
    case 2:
        return faseTematica3();
    case 3:
        return faseTematica4();
    case 4:
        return faseTematica5();
    case 5:
        return faseTematica6();
    case 6:
        return faseTematica7();
    case 7:
        return faseTematica8();
    default:
        return 0;
    }
}

SDL_Texture* personagem (int i) {
    switch (i) {
    case 0:
        return carregarImagem("./media/skins/player/papai noel.png");
    case 1:
        return carregarImagem("./media/skins/player/marinheiro.png");
    case 2:
        return carregarImagem("./media/skins/player/slash.png");
    case 3:
        return carregarImagem("./media/skins/player/batman.png");
    case 4:
        return carregarImagem("./media/skins/player/militar.png");
    case 5:
        return carregarImagem("./media/skins/player/kelly.png");
    case 6:
        return carregarImagem("./media/skins/player/deadpool.png");
    case 7:
        return carregarImagem("./media/skins/player/pikachu.png");
    case 8:
        return carregarImagem("./media/skins/player/superman.png");
    default:
        return NULL;
    }
}

void ganhou(int fase) {
    // variaveis
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true;

    // TEXTURAS
    SDL_Texture* ganhouTexture = carregarImagem("./media/menu/final.png");
    SDL_Texture* personagemTexture = personagem(fase);

    // RECTS
    SDL_Rect ganhouRect = {50, 50, 900, 630};
    SDL_Rect personagem = {425, 300, 150, 150};

    // ANIMAÇAO DO PERSONAGEM
    float angulo = -8;
    float variacaoAngulo = 0.30;

    int x, y;
    int animacao = 0;
    while(running) {
        //verifica os eventos:
        while(SDL_PollEvent(event)) {
            //fechar janela
            closeWindow(event, &running);
            // clique do mouse
            if (event->type == SDL_MOUSEBUTTONDOWN) {
                running = false;
            }
        }

        // atualiza o angulo
        angulo += variacaoAngulo;
        if (angulo > 8 || angulo < -8) {
            variacaoAngulo *= -1;
        }

        // pinta o fundo
        SDL_SetRenderDrawColor(gRenderer, 255, 222, 129, 255);
        SDL_RenderClear(gRenderer);

        SDL_RenderCopy(gRenderer, ganhouTexture, 0, &ganhouRect);
        SDL_RenderCopyEx(gRenderer, personagemTexture, 0, &personagem, angulo, 0, 0);

        SDL_RenderPresent(gRenderer);

        SDL_Delay(20);
    }
    transicao();
    free(event);
}

// return true se precisa ser chamada de novo
int historia() {

    // variaveis
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true;

    // ultima fase carregada
    int ultima = ler() + 1;

    // cria vetores de texture e rect para todas imagens
    SDL_Texture* textures[ultima];
    SDL_Rect rects[ultima];
    bool hover[ultima];

    int i;
    char nome[30];
    for (i = 0; i < ultima; i++) {
        sprintf(nome, "./media/thumbs/0%dfase.png", i+1);
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
    while(running) {
        //verifica os eventos:
        while(SDL_PollEvent(event)) {
            //fechar janela
            closeWindow(event, &running);
            // clique do mouse
            if (event->type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&x, &y);
                for (i = 0; i < ultima; i++) {
                    if (collRectPoint(&rects[i], x, y)) {
                        //ABRE fase
                        transicao();
                        int valor = faseNumero(i);
                        transicao();
                        if (valor < 0) {
                            ganhou(i);
                            if (i >= ultima-1 && i+1 < FASES) {
                                salvar(i+1);
                                return true;
                            }
                        }
                        break;
                    }
                }
            }
            // movimento do mouse
            if (event->type == SDL_MOUSEMOTION) {
                SDL_GetMouseState(&x, &y);
                for (i = 0; i < ultima; i++) {
                    hoverRect(&rects[i], &hover[i], x, y, 20, 20);
                }
            }
        }

        if (animacao < 10*ultima) {
            SDL_SetTextureAlphaMod(textures[animacao/10], (animacao%10*255)/9);
            animacao++;
        }

        // pinta o fundo
        SDL_SetRenderDrawColor(gRenderer, 255, 222, 129, 255);
        SDL_RenderClear(gRenderer);

        // imprime todas imagens
        for (i = 0; i < ultima; i++) {
            SDL_RenderCopy(gRenderer, textures[i], 0, &rects[i]);
        }

        SDL_RenderPresent(gRenderer);

        SDL_Delay(20);
    }
    transicao();
    free(event);
    return false;
}
