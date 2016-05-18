#include <stdio.h>
#include <SDL.h>
#include "SDL_Physics.h"
#include "bool.h"

//esse arquivo contem as funcoes declaradas em "physics.h"

//detecta colisão entre retangulo e parede, e inverte valor da velocidade: (flavio)
void collRectWall(SDL_Rect* rect, Var* varObj, const int* SCREEN_WIDTH, const int* SCREEN_HEIGHT){

    if((rect->x)<0){
        varObj->vX *= (-1);
        rect->x = 0;
    }
    else if((rect->x)+(rect->w) > *SCREEN_WIDTH){
        varObj->vX *= (-1);
        rect->x = *SCREEN_WIDTH - (rect->w);
    }

    if((rect->y)<0){
        varObj->vY *= (-1);
        rect->y = 0;
    }
    else if((rect->y)+(rect->h) > *SCREEN_HEIGHT){
        varObj->vY *= (-1);
        rect->y = *SCREEN_HEIGHT - (rect->h);
    }
}

//atualiza a velocidade de acordo com os comandos contidos na struct Ctrl e com a freqCtrl: (flavio)
void doTheCtrl(Ctrl* ctrlObj, Var* varObj, Hand* handObj){
    static int contFreqCtrl=0;
    contFreqCtrl++;
    if(contFreqCtrl == (handObj->fC)){
        if(ctrlObj->left) varObj->vX -= handObj->hX;
        if(ctrlObj->right) varObj->vX += handObj->hX;
        if(ctrlObj->up) varObj->vY -= handObj->hY;
        if(ctrlObj->down) varObj->vY += handObj->hY;
        contFreqCtrl = 0;
    }
}

//atualiza a posição do objeto de acordo com a velocidade: (flavio)
void velObj(int *pX, int *pY, int *vX, int *vY){
    *pX += *vX;
    *pY += *vY;
}

//atualiza a velocidade do personagem de acordo com a aceleração e com a freqAcel: (flavio)
void acelObj(Var* varObj){
    static int contFreqAcel=0;
    contFreqAcel++;
    if(contFreqAcel == (varObj->fA)){
        varObj->vX += varObj->aX;
        varObj->vY += varObj->aY;
        contFreqAcel = 0;
    }
}

//pega os comandos para o objeto:
void ctrlObj(SDL_Event* event, Ctrl* ctrlObj){

    if (event->type == SDL_KEYDOWN) {
        switch ((*event).key.keysym.sym) {
            case SDLK_LEFT:
                ctrlObj->left = true;
            break;
            case SDLK_RIGHT:
                ctrlObj->right = true;
            break;
            case SDLK_DOWN:
                ctrlObj->down = true;
            break;
            case SDLK_UP:
                ctrlObj->up = true;
            break;
        }
    } else if (event->type == SDL_KEYUP) {
        switch ((*event).key.keysym.sym) {
            case SDLK_LEFT:
                ctrlObj->left = false;
            break;
            case SDLK_RIGHT:
                ctrlObj->right = false;
            break;
            case SDLK_DOWN:
                ctrlObj->down = false;
            break;
            case SDLK_UP:
                ctrlObj->up = false;
            break;
        }
    }
}
