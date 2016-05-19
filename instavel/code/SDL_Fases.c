//esse arquivo contem as fases do jogo.

#include <stdio.h>
#include <SDL.h>
#include "SDL_Main.h"
#include "SDL_Fases.h"
#include "SDL_Physics.h"
#include "bool.h"

//fases:
void fase1(const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT){
    //declarem td aqui em cima:
    SDL_Event event;
    SDL_Rect rect = {100, 100, 50, 50}; //retangulo azul

    bool running = true; //mantem o laço principal rodando
    Var varRect = {5,0,0,1,3}; //variação do retangulo azul (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handRect = {1,1,3}; //struct de controle do retangulo azul (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlRect = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    while(running){
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

        //retangulo azul:
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
        SDL_RenderFillRect(gRenderer, &rect);

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //interacoes fisicas:
        collRectWall(&rect, &varRect, SCREEN_WIDTH, SCREEN_HEIGHT);
        acelObj(&varRect);
        doTheCtrl(&ctrlRect, &varRect, &handRect);
        velObj(&rect.x, &rect.y, &(varRect.vX), &(varRect.vY));

        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
}
//-----------------
