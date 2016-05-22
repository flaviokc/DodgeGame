//esse arquivo contem as fases do jogo.

#include <stdio.h>
#include <SDL.h>
#include "SDL_Main.h"
#include "SDL_Fases.h"
#include "SDL_Physics.h"
#include "bool.h"

//fases:

void fasebug(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT){
    //declarem td aqui em cima:
    SDL_Event event;
    SDL_Rect rect = {100, 100, 50, 50}; //retangulo azul

    bool running = true; //mantem o laço principal rodando
    Var varRect = {2,0,0,1,3}; //variação do retangulo azul (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handRect = {1,1,5}; //struct de controle do retangulo azul (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlRect = {false, false, false, false}; //struct bool direçoes de controle
    int tempo = 30; //duração da fase em segundos
    int contador; //contador para o tempo
    SDL_Rect tempoRect; //rect com posicao e tamanho da tempoTexture
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    //-----------------------------------

    // inicia o y do tempoRect que sera fixo
    tempoRect.y = 200;
    // registra o tempo atual no contador
    contador = SDL_GetTicks();
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempo, tempoColor, &tempoRect, SCREEN_WIDTH);

    while(tempo >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(&event)){
            //fechar janela
            closeWindow(&event, &running);
            //pega os comandos para o objeto:
            ctrlObj(&event, &ctrlRect);
        }
        //fundo verde:
        SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
        SDL_RenderClear(gRenderer);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //retangulo azul:
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
        SDL_RenderFillRect(gRenderer, &rect);

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //interacoes fisicas:
        collRectWall(&rect, &varRect, SCREEN_WIDTH, SCREEN_HEIGHT);
        acelObj(&varRect);
        doTheCtrl1(&ctrlRect, &varRect, &handRect);
        velObj(&rect.x, &rect.y, &varRect.vX, &(varRect.vY));

        // verifica se passou um segundo desde o ultimo registro do tempo
        if (SDL_GetTicks() - contador >= 1000) {
            tempo--;
            //SDL_DestroyTexture(tempoTexture);
            tempoTexture = criarTexture(tempo, tempoColor, &tempoRect, SCREEN_WIDTH);
            contador = SDL_GetTicks();
        }

        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
}

void faseExemplo(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT){
    //declarem td aqui em cima:
    SDL_Event event;
    bool running = true; //mantem o laço principal rodando
    int cantoInferiorDireito[2] = {*SCREEN_WIDTH - 80, *SCREEN_HEIGHT - 80};
    int tantoFaz; //fiz isso só pra ficar claro que tanto faz kkk

    //personagem:
    SDL_Rect personagem = {500, 550, 60, 60}; //retangulo azul
    Var varPersonagem = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,6}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1:
    SDL_Rect inimigo1 = {0, 0, 80, 80}; //retangulo azul
    Var varInimigo1 = {0,7,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo1 = {0, 0, *SCREEN_WIDTH, *SCREEN_HEIGHT};
    //-----------------------------------

    //enemy2:
    SDL_Rect inimigo2 = {cantoInferiorDireito[0], cantoInferiorDireito[1], 80, 80}; //retangulo azul
    Var varInimigo2 = {0,-7,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo2 = {0, 0, *SCREEN_WIDTH, *SCREEN_HEIGHT};
    //-----------------------------------

    //enemy3:
    SDL_Rect inimigo3 = {100, 500, 50, 50}; //retangulo azul
    Var varInimigo3 = {5,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    while(running){
        //verifica os eventos:
        while(SDL_PollEvent(&event)){
            //fechar janela
            closeWindow(&event, &running);
            //pega os comandos para o personagem:
            ctrlObj(&event, &ctrlPersonagem);
        }

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)||coll2Circles(&personagem, &inimigo2)||coll2Circles(&personagem, &inimigo3)) {
            //fundo amarelo:
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
        } else {
            //fundo verde:
            SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
        }
        //pinta o fundo
        SDL_RenderClear(gRenderer);

        //print personagem:
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
        SDL_RenderFillRect(gRenderer, &personagem);

        //print inimigo1:
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(gRenderer, &inimigo1);

        //print inimigo2:
        SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
        SDL_RenderFillRect(gRenderer, &inimigo2);

        //print inimigo3:
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderFillRect(gRenderer, &inimigo3);

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //personagem (interacoes fisicas):
        collRectWall(&personagem, &varPersonagem, SCREEN_WIDTH, SCREEN_HEIGHT);
        acelObj(&varPersonagem);
        doTheCtrl1(&ctrlPersonagem, &varPersonagem, &handPersonagem);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1 (interacoes fisicas):
        collEnemyWall(&inimigo1, &rotaInimigo1, &varInimigo1);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);

        //inimigo2 (interacoes fisicas):
        collEnemyWall(&inimigo2, &rotaInimigo2, &varInimigo2);
        velObj(&inimigo2.x, &inimigo2.y, &varInimigo2.vX, &varInimigo2.vY);

        //inimigo3 (interações físicas):
        collRectWall(&inimigo3, &varInimigo3, SCREEN_WIDTH, SCREEN_HEIGHT);
        acelObj(&varInimigo3);
        velObj(&inimigo3.x, &inimigo3.y, &varInimigo3.vX, &varInimigo3.vY);
        //printf("%d, %d\n", inimigo1.x, inimigo1.y);
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
}
//-----------------
