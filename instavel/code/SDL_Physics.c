#include <stdio.h>
#include <SDL.h>
#include "SDL_Physics.h"
#include "SDL_Media.h"
#include "bool.h"

//esse arquivo contem as funcoes declaradas em "physics.h"

//detecta colisão entre retangulo e parede, e inverte valor da velocidade: (flavio)
void collRectWall(SDL_Rect* rect, Var* varObj){
    if((rect->x)<0){
        varObj->vX *= (-1);
        rect->x = 0;
    }
    else if((rect->x)+(rect->w) > SCREEN_WIDTH){
        varObj->vX *= (-1);
        rect->x = SCREEN_WIDTH - (rect->w);
    }

    if((rect->y)<0){
        varObj->vY *= (-1);
        rect->y = 0;
    }
    else if((rect->y)+(rect->h) > SCREEN_HEIGHT){
        varObj->vY *= (-1);
        rect->y = SCREEN_HEIGHT - (rect->h);
    }
}

//atualiza a velocidade de acordo com os comandos contidos na struct Ctrl e com a freqCtrl: (flavio)
void doTheCtrl(Ctrl* ctrlObj, Var* varObj, Hand* handObj, int contWhile){
    if(!(contWhile % handObj->fC)){
        if(ctrlObj->left) varObj->vX -= handObj->hX;
        if(ctrlObj->right) varObj->vX += handObj->hX;
        if(ctrlObj->up) varObj->vY -= handObj->hY;
        if(ctrlObj->down) varObj->vY += handObj->hY;
    }
}

//atualiza a posição do objeto de acordo com a velocidade: (flavio)
void velObj(int *pX, int *pY, int *vX, int *vY){
    *pX += *vX;
    *pY += *vY;
}

//atualiza a velocidade do personagem de acordo com a aceleração e com a freqAcel: (flavio)
void acelObj(Var* varObj, int contWhile){
    if(!(contWhile % varObj->fA)){
        varObj->vX += varObj->aX;
        varObj->vY += varObj->aY;
    }
}

//pega os comandos para o objeto:
void ctrlObj(SDL_Event* event, Ctrl* ctrlObj){

    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
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
        switch (event->key.keysym.sym) {
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

//pega os comandos para o objeto extra:
void ctrlExtra(SDL_Event* event, Ctrl* ctrlObj){

    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_a:
                ctrlObj->left = true;
            break;
            case SDLK_d:
                ctrlObj->right = true;
            break;
            case SDLK_s:
                ctrlObj->down = true;
            break;
            case SDLK_w:
                ctrlObj->up = true;
            break;
        }
    } else if (event->type == SDL_KEYUP) {
        switch (event->key.keysym.sym) {
            case SDLK_a:
                ctrlObj->left = false;
            break;
            case SDLK_d:
                ctrlObj->right = false;
            break;
            case SDLK_s:
                ctrlObj->down = false;
            break;
            case SDLK_w:
                ctrlObj->up = false;
            break;
        }
    }
}

//verifica se duas circunferências se intersectam: (flavio)
bool coll2Circles(SDL_Rect* rect1, SDL_Rect* rect2) {
    int dX = (rect2->x + (rect2->w)/2 - (rect1->x + (rect1->w)/2));
    int dY = (rect2->y + (rect2->h)/2 - (rect1->y + (rect1->h)/2));
    int somaRaios = (rect1->w + rect2->w)/2;
    if(dX*dX + dY*dY > somaRaios*somaRaios){
        return false;
    } else {
        return true;
    }
}

//verifica se o inimigo bateu na parede e inverte sua velocidade perpendicularmente: (flavio)
void collEnemyWall(SDL_Rect* rect, Rota* rotaObj, Var* varObj){
    int aux;
    //verifica se bateu no canto superior esquerdo:
    if((rect->x <= rotaObj->x1)&&(rect->y <= rotaObj->y1)) {
        //reposiciona o rect:
        rect->x = rotaObj->x1;
        rect->y = rotaObj->y1;
        aux = varObj->vX;
        //transforma perpendicularmente a velocidade:
        varObj->vX = varObj->vY * (-1);
        varObj->vY = aux * (-1);
    } else if((rect->x >= rotaObj->x2)&&(rect->y >= rotaObj->y2)) { //verifica se bateu no canto inferior direito:
        //reposiciona o rect:
        rect->x = rotaObj->x2;
        rect->y = rotaObj->y2;
        aux = varObj->vX;
        //transforma perpendicularmente a velocidade:
        varObj->vX = varObj->vY * (-1);
        varObj->vY = aux * (-1);
    } else if((rect->x <= rotaObj->x1)&&(rect->y >= rotaObj->y2)) { //verifica se bateu no canto inferior esquerdo:
        //reposiciona o rect:
        rect->x = rotaObj->x1;
        rect->y = rotaObj->y2;
        aux = varObj->vX;
        //transforma perpendicularmente a velocidade:
        varObj->vX = varObj->vY;
        varObj->vY = aux;
    } else if((rect->x >= rotaObj->x2)&&(rect->y <= rotaObj->y1)) { //verifica se bateu no canto superior direito:
        //reposiciona o rect:
        rect->x = rotaObj->x2;
        rect->y = rotaObj->y1;
        aux = varObj->vX;
        //transforma perpendicularmente a velocidade:
        varObj->vX = varObj->vY;
        varObj->vY = aux;
    }
}

//atualiza o contWhile
void atualizaContWhile(int* contWhile, int mmcMaisUm){
    if(*contWhile >= mmcMaisUm){ //compara com o (MMC(todos os freqAcel e freqCtrl) + 1)
        *contWhile = 1;
    }
    (*contWhile)++;
}

/*altera aleatoriamente ou não a aceleracao do objeto de acordo com o parametro:
parametro -->{0,1,2,3,4}
resultado -->{aleatorio, esquerda, direita, cima, baixo}
retorna o numero referente a direcao em que mudou seguindo o padrao acima.
retorna 0 se não mudou a direcao.
(Essa funcao sempre altera o modulo da aceleracao para 1) */
int alteraGravidade(Var* varObj, int tempoRestante, int freqMudGrav, int parametro){
    int aleatorio;
    if(!(tempoRestante % freqMudGrav)){ //verifica se é pra mudar a gravidade
        if(!parametro){ //verifica se eh pra mudar aleatoriamente a gravidade
            aleatorio = (rand()%4)+1; //gera um numero aleatorio entre 1 e 4
            //altera a aceleração de acordo com o numero aleatorio seguindo o padrao especificado acima.
            //caso o numero aleatorio ja corresponda a direcao atual, escolhe-se a direcao do aleatorio+1
            switch(aleatorio){
                //esquerda:
                case 1:
                    if(varObj->aX == -1){
                        varObj->aX = 1;
                        varObj->aY = 0;
                        aleatorio = 2;
                    } else{
                        varObj->aX = -1;
                        varObj->aY = 0;
                    }
                break;
                //direita:
                case 2:
                    if(varObj->aX == 1){
                        varObj->aY = -1;
                        varObj->aX = 0;
                        aleatorio = 3;
                    } else{
                        varObj->aX = 1;
                        varObj->aY = 0;
                    }
                break;
                //cima:
                case 3:
                    if(varObj->aY == -1){
                        varObj->aY = 1;
                        varObj->aX = 0;
                        aleatorio = 4;
                    } else{
                        varObj->aY = -1;
                        varObj->aX = 0;
                    }
                break;
                //baixo:
                case 4:
                    if(varObj->aY == 1){
                        varObj->aX = -1;
                        varObj->aY = 0;
                        aleatorio = 1;
                    } else{
                        varObj->aY = 1;
                        varObj->aX = 0;
                    }
                break;
            }
            //retorna a nova direcao
            return aleatorio;
        } else{
            //altera a aceleração de acordo com o numero aleatorio seguindo o padrao especificado acima.
            switch(parametro){
                //esquerda:
                case 1:
                    varObj->aX = -1;
                    varObj->aY = 0;
                break;
                //direita:
                case 2:
                    varObj->aX = 1;
                    varObj->aY = 0;
                break;
                //cima:
                case 3:
                    varObj->aY = -1;
                    varObj->aX = 0;
                break;
                //baixo:
                case 4:
                    varObj->aY = 1;
                    varObj->aX = 0;
                break;
            }
            //retorna a nova direcao
            return parametro;
        }
    } else{
        //a direcao nao foi alterada
        return 0;
    }
}
