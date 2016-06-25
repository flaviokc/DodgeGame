//esse arquivo contem as fases do jogo.

#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include "SDL_Media.h"
#include "SDL_Fases.h"
#include "SDL_Physics.h"
#include "bool.h"

//fases:
void fasebug(){
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEMPO:
    int tempoRestante = 20; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-----------------------------------


    //OBJETOS:

    //retangulo azul:
    SDL_Rect rect = {100, 100, 50, 50}; //retangulo azul
    Var varRect = {2,0,0,1,3}; //variação do retangulo azul (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handRect = {1,1,5}; //struct de controle do retangulo azul (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlRect = {false, false, false, false}; //struct bool direçoes de controle


    //--------------------------

    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o objeto:
            ctrlObj(event, &ctrlRect);
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

        //INTERACOES FISICAS:

        //personagem:
        collRectWall(&rect, &varRect);
        acelObj(&varRect, contWhile);
        doTheCtrl(&ctrlRect, &varRect, &handRect, contWhile);
        velObj(&rect.x, &rect.y, &varRect.vX, &(varRect.vY));

        //----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    free(event);
}

void faseExemplo(){

    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEMPO
    int tempoRestante = 20; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {500, 550, 60, 60};
    Var varPersonagem = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1: //rotador
    SDL_Rect inimigo1 = {0, 0, 80, 80};
    Var varInimigo1 = {0,7,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo1 = {0, 0, SCREEN_WIDTH - inimigo1.w, SCREEN_HEIGHT - inimigo1.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy2: //rotador
    SDL_Rect inimigo2 = {SCREEN_WIDTH - 80, SCREEN_HEIGHT - 80, 80, 80};
    Var varInimigo2 = {0,-7,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo2 = {0, 0, SCREEN_WIDTH - inimigo2.w, SCREEN_HEIGHT - inimigo2.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy3: //quicador
    SDL_Rect inimigo3 = {100, 500, 50, 50};
    Var varInimigo3 = {5,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
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

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
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
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS
        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        acelObj(&varPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1:
        collEnemyWall(&inimigo1, &rotaInimigo1, &varInimigo1);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);

        //inimigo2:
        collEnemyWall(&inimigo2, &rotaInimigo2, &varInimigo2);
        velObj(&inimigo2.x, &inimigo2.y, &varInimigo2.vX, &varInimigo2.vY);

        //inimigo3:
        collRectWall(&inimigo3, &varInimigo3);
        acelObj(&varInimigo3, contWhile);
        velObj(&inimigo3.x, &inimigo3.y, &varInimigo3.vX, &varInimigo3.vY);
        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    free(event);
}

int faseTutorial1(){

    //tutorial1
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/tutorial.png");
    SDL_Texture* personagemTexture = carregarImagem("./media/skins/player/blacksheep.png");
    SDL_Texture* inimigoTexture = carregarImagem("./media/skins/enemy/porca.png");

    //MUSICA
    play_Music("./media/songs/Just_Like_Fire.ogg");

    //TEMPO
    int tempoRestante = 20; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {500, 550, 85, 85}; //personagem
    Var varPersonagem = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1: //quicador
    SDL_Rect inimigo1 = {100, 500, 85, 85};
    Var varInimigo1 = {-5,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
        }

        //OBJETOS:
        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagem:
        SDL_RenderCopy(gRenderer, personagemTexture, 0, &personagem);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo1);
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        acelObj(&varPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1:
        collRectWall(&inimigo1, &varInimigo1);
        acelObj(&varInimigo1, contWhile);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);
        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(inimigoTexture);
    SDL_DestroyTexture(personagemTexture);
    free(event);

    return tempoRestante;
}

int faseTutorial2(){
    //tutorial2
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/tutorial.png");
    SDL_Texture* personagemTexture = carregarImagem("./media/skins/player/blacksheep.png");
    SDL_Texture* inimigoTexture = carregarImagem("./media/skins/enemy/porca.png");

    //MUSICA
    play_Music("./media/songs/All_About_That_Bass.ogg");

    //TEMPO
    int tempoRestante = 20; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {500, 550, 85, 85}; //personagem
    Var varPersonagem = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle

    //enemy1:
    SDL_Rect inimigo1 = {0, 0, 80, 80}; //personagem
    Var varInimigo1 = {0,7,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo1 = {0, 0, SCREEN_WIDTH - inimigo1.w, SCREEN_HEIGHT - inimigo1.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
        }

        //OBJETOS:
        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagem:
        SDL_RenderCopy(gRenderer, personagemTexture, 0, &personagem);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo1);
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        acelObj(&varPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1:
        collEnemyWall(&inimigo1, &rotaInimigo1, &varInimigo1);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
            //altera a gravidade aleatoriamente a cada 7 segundos:
            alteraGravidade(&varPersonagem, tempoRestante, 7, 0);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(inimigoTexture);
    SDL_DestroyTexture(personagemTexture);
    free(event);

    return tempoRestante;
}

int faseTutorial3(){
    //tutorial3
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/tutorial.png");
    SDL_Texture* personagemTexture = carregarImagem("./media/skins/player/blacksheep.png");
    SDL_Texture* inimigoTexture = carregarImagem("./media/skins/enemy/porca.png");

    //MUSICA
    play_Music("./media/songs/Shake_It_Off.ogg");

    //TEMPO
    int tempoRestante = 20; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {500, 550, 85, 85}; //personagem
    Var varPersonagem = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1:
    SDL_Rect inimigo1 = {0, 0, 80, 80}; //personagem
    Var varInimigo1 = {0,7,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo1 = {0, 0, SCREEN_WIDTH - inimigo1.w, SCREEN_HEIGHT - inimigo1.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy2:
    SDL_Rect inimigo2 = {SCREEN_WIDTH - 80, SCREEN_HEIGHT - 80, 80, 80}; //personagem
    Var varInimigo2 = {0,-7,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo2 = {0, 0, SCREEN_WIDTH - inimigo2.w, SCREEN_HEIGHT - inimigo2.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy3:
    SDL_Rect inimigo3 = {700, 200, 50, 50}; //personagem
    Var varInimigo3 = {5,-5,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
        }

        //OBJETOS:
        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagem:
        SDL_RenderCopy(gRenderer, personagemTexture, 0, &personagem);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo1);

        //print inimigo2:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo2);

        //print inimigo3:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo3);
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)||coll2Circles(&personagem, &inimigo2)||coll2Circles(&personagem, &inimigo3)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        acelObj(&varPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1:
        collEnemyWall(&inimigo1, &rotaInimigo1, &varInimigo1);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);

        //inimigo2:
        collEnemyWall(&inimigo2, &rotaInimigo2, &varInimigo2);
        velObj(&inimigo2.x, &inimigo2.y, &varInimigo2.vX, &varInimigo2.vY);

        //inimigo3:
        collRectWall(&inimigo3, &varInimigo3);
        velObj(&inimigo3.x, &inimigo3.y, &varInimigo3.vX, &varInimigo3.vY);
        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(inimigoTexture);
    SDL_DestroyTexture(personagemTexture);
    free(event);
    return tempoRestante;
}

//quando a fase receber o tema pode trocar o nome da funcao.
//natal
int faseTematica1(){
    //tematica1
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/natal.png");
    SDL_Texture* personagemTexture = carregarImagem("./media/skins/player/papai noel.png");
    SDL_Texture* inimigoTexture = carregarImagem("./media/skins/enemy/goblin.png");

    //MUSICA
    play_Music("./media/songs/Still_Into_You.ogg");

    //TEMPO
    int tempoRestante = 30; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {460, 500, 85, 85}; //personagem
    Var varPersonagem = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1: rotador
    SDL_Rect inimigo1 = {0, 0, 80, 80}; //personagem
    Var varInimigo1 = {7,0,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo1 = {0, 0, SCREEN_WIDTH - inimigo1.w, SCREEN_HEIGHT - inimigo1.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy2: rotador
    SDL_Rect inimigo2 = {SCREEN_WIDTH - 80, 0, 80, 80}; //personagem
    Var varInimigo2 = {-7,0,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo2 = {0, 0, SCREEN_WIDTH - inimigo2.w, SCREEN_HEIGHT - inimigo2.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy3: rotador
    SDL_Rect inimigo3 = {0, SCREEN_HEIGHT - 80, 80, 80}; //personagem
    Var varInimigo3 = {7,0,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo3 = {0, 0, SCREEN_WIDTH - inimigo3.w, SCREEN_HEIGHT - inimigo3.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy4: rotador
    SDL_Rect inimigo4 = {SCREEN_WIDTH - 80, SCREEN_HEIGHT - 80, 80, 80}; //personagem
    Var varInimigo4 = {-7,0,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo4 = {0, 0, SCREEN_WIDTH - inimigo4.w, SCREEN_HEIGHT - inimigo4.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy5: flutuante
    SDL_Rect inimigo5 = {700, 200, 50, 50}; //personagem
    Var varInimigo5 = {5,-5,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
        }

        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagem:
        SDL_RenderCopy(gRenderer, personagemTexture, 0, &personagem);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo1);

        //print inimigo2:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo2);

        //print inimigo3:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo3);

        //print inimigo4:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo4);

        //print inimigo5:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo5);
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)||coll2Circles(&personagem, &inimigo2)||coll2Circles(&personagem, &inimigo3)||coll2Circles(&personagem, &inimigo4)||coll2Circles(&personagem, &inimigo5)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        acelObj(&varPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1:
        collEnemyWall(&inimigo1, &rotaInimigo1, &varInimigo1);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);

        //inimigo2:
        collEnemyWall(&inimigo2, &rotaInimigo2, &varInimigo2);
        velObj(&inimigo2.x, &inimigo2.y, &varInimigo2.vX, &varInimigo2.vY);

        //inimigo3:
        collEnemyWall(&inimigo3, &rotaInimigo3, &varInimigo3);
        velObj(&inimigo3.x, &inimigo3.y, &varInimigo3.vX, &varInimigo3.vY);

        //inimigo4:
        collEnemyWall(&inimigo4, &rotaInimigo4, &varInimigo4);
        velObj(&inimigo4.x, &inimigo4.y, &varInimigo4.vX, &varInimigo4.vY);

        //inimigo5:
        collRectWall(&inimigo5, &varInimigo5);
        velObj(&inimigo5.x, &inimigo5.y, &varInimigo5.vX, &varInimigo5.vY);

        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(inimigoTexture);
    SDL_DestroyTexture(personagemTexture);
    free(event);
}

//marinheiro
int faseTematica5(){
    //tematica5
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/mar2.png");
    SDL_Texture* personagemTexture = carregarImagem("./media/skins/player/marinheiro.png");
    SDL_Texture* inimigoTexture = carregarImagem("./media/skins/enemy/bacteria.png");

    //MUSICA
    play_Music("./media/songs/See_You_Again.ogg");

    //TEMPO
    int tempoRestante = 20; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {500, 550, 85, 85}; //personagem
    Var varPersonagem = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //----------------------------------

    //enemy1: //quicador
    SDL_Rect inimigo1 = {100, 500, 85, 85};
    Var varInimigo1 = {-5,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------


    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
        }

        //OBJETOS:
        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagem:
        SDL_RenderCopy(gRenderer, personagemTexture, 0, &personagem);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo1);
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        acelObj(&varPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1:
        collRectWall(&inimigo1, &varInimigo1);
        acelObj(&varInimigo1, contWhile);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);
        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
            //altera a gravidade aleatoriamente a cada 7 segundos:
            alteraGravidade(&varInimigo1, tempoRestante, 5, 0);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(inimigoTexture);
    SDL_DestroyTexture(personagemTexture);
    free(event);
}

//slash
int faseTematica3(){
    //tematica3
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/rock.png");
    SDL_Texture* personagemTexture = carregarImagem("./media/skins/player/slash.png");
    SDL_Texture* inimigo1Texture = carregarImagem("./media/skins/enemy/inimigorock2.png");
    SDL_Texture* inimigo2Texture = carregarImagem("./media/skins/enemy/inimigorock1.png");

    //MUSICA
    play_Music("./media/songs/Smells_Like_Teen_Spirit.ogg");

    //TEMPO
    int tempoRestante = 20; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {500, 550, 85, 85}; //personagem
    Var varPersonagem = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1: //quicador
    SDL_Rect inimigo1 = {100, 500, 85, 85};
    Var varInimigo1 = {-5,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy2: //quicador
    SDL_Rect inimigo2 = {350, 150, 85, 85};
    Var varInimigo2 = {-5,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
        }

        //OBJETOS:
        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagem:
        SDL_RenderCopy(gRenderer, personagemTexture, 0, &personagem);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigo1Texture, 0, &inimigo1);
        //-------------------

        //print inimigo2:
        SDL_RenderCopy(gRenderer, inimigo2Texture, 0, &inimigo2);
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)||coll2Circles(&personagem, &inimigo2)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        acelObj(&varPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1:
        collRectWall(&inimigo1, &varInimigo1);
        acelObj(&varInimigo1, contWhile);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);
        //-----------------------

        //inimigo2:
        collRectWall(&inimigo2, &varInimigo2);
        acelObj(&varInimigo2, contWhile);
        velObj(&inimigo2.x, &inimigo2.y, &varInimigo2.vX, &varInimigo2.vY);
        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(inimigo1Texture);
    SDL_DestroyTexture(inimigo2Texture);
    SDL_DestroyTexture(personagemTexture);
    free(event);

    return tempoRestante;
}

//batman
int faseTematica4(){
    //tematica4
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/cidade_batman.png");
    SDL_Texture* personagemTexture = carregarImagem("./media/skins/player/batman.png");
    SDL_Texture* inimigo1Texture = carregarImagem("./media/skins/player/superman.png");
    SDL_Texture* inimigo2Texture = carregarImagem("./media/skins/enemy/apocalipse.png");
    SDL_Texture* inimigo3Texture = carregarImagem("./media/skins/enemy/coringa.png");

    //MUSICA
    play_Music("./media/songs/Confident.ogg");

    //TEMPO
    int tempoRestante = 30; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {460, 500, 85, 85}; //personagem
    Var varPersonagem = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1: rotador
    SDL_Rect inimigo1 = {0, 0, 80, 80}; //personagem
    Var varInimigo1 = {0,7,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo1 = {0, 0, SCREEN_WIDTH - inimigo1.w, SCREEN_HEIGHT - inimigo1.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy2: rotador
    SDL_Rect inimigo2 = {200, 0, 80, 80}; //personagem
    Var varInimigo2 = {7,0,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo2 = {200, 0, SCREEN_WIDTH - inimigo2.w - 200, SCREEN_HEIGHT - inimigo2.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy3: rotador
    SDL_Rect inimigo3 = {0, 200, 80, 80}; //personagem
    Var varInimigo3 = {0,7,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo3 = {0, 200, SCREEN_WIDTH - inimigo3.w, SCREEN_HEIGHT - inimigo3.h - 200}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------


    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
        }

        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagem:
        SDL_RenderCopy(gRenderer, personagemTexture, 0, &personagem);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigo1Texture, 0, &inimigo1);

        //print inimigo2:
        SDL_RenderCopy(gRenderer, inimigo2Texture, 0, &inimigo2);

        //print inimigo3:
        SDL_RenderCopy(gRenderer, inimigo3Texture, 0, &inimigo3);

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)||coll2Circles(&personagem, &inimigo2)||coll2Circles(&personagem, &inimigo3)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        acelObj(&varPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1:
        collEnemyWall(&inimigo1, &rotaInimigo1, &varInimigo1);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);

        //inimigo2:
        collEnemyWall(&inimigo2, &rotaInimigo2, &varInimigo2);
        velObj(&inimigo2.x, &inimigo2.y, &varInimigo2.vX, &varInimigo2.vY);

        //inimigo3:
        collEnemyWall(&inimigo3, &rotaInimigo3, &varInimigo3);
        velObj(&inimigo3.x, &inimigo3.y, &varInimigo3.vX, &varInimigo3.vY);

        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(inimigo1Texture);
    SDL_DestroyTexture(inimigo2Texture);
    SDL_DestroyTexture(inimigo3Texture);
    SDL_DestroyTexture(personagemTexture);
    free(event);

    return tempoRestante;
}

//militar
int faseTematica7(){
    //tematica7
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/tanque.png");
    SDL_Texture* personagemTexture = carregarImagem("./media/skins/player/militar.png");
    SDL_Texture* inimigoTexture = carregarImagem("./media/skins/enemy/nazista.png");

    //MUSICA
    play_Music("./media/songs/California_Gurls.ogg");

    //TEMPO
    int tempoRestante = 20; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {500, 550, 85, 85}; //personagem
    Var varPersonagem = {0,0,0,0,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1: //quicador em baixo
    SDL_Rect inimigo1 = {450, 300, 50, 50};
    Var varInimigo1 = {-3,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy2: //quicador na direita
    SDL_Rect inimigo2 = {450, 300, 50, 50};
    Var varInimigo2 = {0,-3,1,0,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy3: //quicador na esquerda
    SDL_Rect inimigo3 = {450, 300, 50, 50};
    Var varInimigo3 = {0,3,-1,0,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy4: //quicador em cima
    SDL_Rect inimigo4 = {450, 300, 50, 50};
    Var varInimigo4 = {3,0,0,-1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
        }

        //OBJETOS:
        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagem:
        SDL_RenderCopy(gRenderer, personagemTexture, 0, &personagem);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo1);
        //-------------------

        //print inimigo2:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo2);
        //-------------------

        //print inimigo3:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo3);
        //-------------------

        //print inimigo4:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo4);
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)||coll2Circles(&personagem, &inimigo2)||coll2Circles(&personagem, &inimigo3)||coll2Circles(&personagem, &inimigo4)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1:
        collRectWall(&inimigo1, &varInimigo1);
        acelObj(&varInimigo1, contWhile);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);
        //-----------------------

        //inimigo2:
        collRectWall(&inimigo2, &varInimigo2);
        acelObj(&varInimigo2, contWhile);
        velObj(&inimigo2.x, &inimigo2.y, &varInimigo2.vX, &varInimigo2.vY);
        //-----------------------

        //inimigo3:
        collRectWall(&inimigo3, &varInimigo3);
        acelObj(&varInimigo3, contWhile);
        velObj(&inimigo3.x, &inimigo3.y, &varInimigo3.vX, &varInimigo3.vY);
        //-----------------------

        //inimigo4:
        collRectWall(&inimigo4, &varInimigo4);
        acelObj(&varInimigo4, contWhile);
        velObj(&inimigo4.x, &inimigo4.y, &varInimigo4.vX, &varInimigo4.vY);
        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(inimigoTexture);
    SDL_DestroyTexture(personagemTexture);
    free(event);
}

//kelly
int faseTematica2(){
    //tematica2
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/kelly.png");
    SDL_Texture* personagemTexture = carregarImagem("./media/skins/player/kelly.png");
    SDL_Texture* inimigo1Texture = carregarImagem("./media/skins/enemy/inimigokelly2.png");
    SDL_Texture* inimigo2Texture = carregarImagem("./media/skins/enemy/inimigokelly1.png");

    //MUSICA
    play_Music("./media/songs/My_Life_Would_Suck_Without_You.ogg");

    //TEMPO
    int tempoRestante = 20; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {500, 550, 85, 85}; //personagem
    Var varPersonagem = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1: //quicador
    SDL_Rect inimigo1 = {100, 500, 85, 85};
    Var varInimigo1 = {-5,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy2: //quicador
    SDL_Rect inimigo2 = {350, 300, 85, 85};
    Var varInimigo2 = {-5,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy3: //quicador
    SDL_Rect inimigo3 = {600, 100, 85, 85};
    Var varInimigo3 = {-5,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
        }

        //OBJETOS:
        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagem:
        SDL_RenderCopy(gRenderer, personagemTexture, 0, &personagem);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigo1Texture, 0, &inimigo1);
        //-------------------

        //print inimigo2:
        SDL_RenderCopy(gRenderer, inimigo2Texture, 0, &inimigo2);
        //-------------------

        //print inimigo3:
        SDL_RenderCopy(gRenderer, inimigo1Texture, 0, &inimigo3);
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)||coll2Circles(&personagem, &inimigo2)||coll2Circles(&personagem, &inimigo3)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        acelObj(&varPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1:
        collRectWall(&inimigo1, &varInimigo1);
        acelObj(&varInimigo1, contWhile);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);
        //-----------------------

        //inimigo2:
        collRectWall(&inimigo2, &varInimigo2);
        acelObj(&varInimigo2, contWhile);
        velObj(&inimigo2.x, &inimigo2.y, &varInimigo2.vX, &varInimigo2.vY);
        //-----------------------

        //inimigo3:
        collRectWall(&inimigo3, &varInimigo3);
        acelObj(&varInimigo3, contWhile);
        velObj(&inimigo3.x, &inimigo3.y, &varInimigo3.vX, &varInimigo3.vY);
        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(inimigo1Texture);
    SDL_DestroyTexture(inimigo2Texture);
    SDL_DestroyTexture(personagemTexture);
    free(event);

    return tempoRestante;
}

//deadpool
int faseTematica6(){
    //tematica6
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/cidade_deadpool.png");
    SDL_Texture* personagemTexture = carregarImagem("./media/skins/player/deadpool.png");
    SDL_Texture* inimigoTexture = carregarImagem("./media/skins/enemy/bacteria.png");

    //MUSICA
    play_Music("./media/songs/I_Kissed_A_Girl.ogg");

    //TEMPO
    int tempoRestante = 20; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {500, 550, 85, 85}; //personagem
    Var varPersonagem = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1: flutuante
    SDL_Rect inimigo1 = {500, 300, 50, 50}; //personagem
    Var varInimigo1 = {5,-5,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy2: flutuante
    SDL_Rect inimigo2 = {700, 400, 50, 50}; //personagem
    Var varInimigo2 = {5,4,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy3: flutuante
    SDL_Rect inimigo3 = {400, 200, 50, 50}; //personagem
    Var varInimigo3 = {4,-5,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy4: flutuante
    SDL_Rect inimigo4 = {200, 200, 50, 50}; //personagem
    Var varInimigo4 = {3,-5,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
        }

        //OBJETOS:
        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagem:
        SDL_RenderCopy(gRenderer, personagemTexture, 0, &personagem);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo1);

        //print inimigo2:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo2);

        //print inimigo3:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo3);
        //-------------------

        //print inimigo4:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo4);
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)||coll2Circles(&personagem, &inimigo2)||coll2Circles(&personagem, &inimigo3)||coll2Circles(&personagem, &inimigo4)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        acelObj(&varPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1: flutuante
        collRectWall(&inimigo1, &varInimigo1);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);
        //-----------------------

        //inimigo2: flutuante
        collRectWall(&inimigo2, &varInimigo2);
        velObj(&inimigo2.x, &inimigo2.y, &varInimigo2.vX, &varInimigo2.vY);
        //-----------------------

        //inimigo3: flutuante
        collRectWall(&inimigo3, &varInimigo3);
        velObj(&inimigo3.x, &inimigo3.y, &varInimigo3.vX, &varInimigo3.vY);
        //-----------------------

        //inimigo4: flutuante
        collRectWall(&inimigo4, &varInimigo4);
        velObj(&inimigo4.x, &inimigo4.y, &varInimigo4.vX, &varInimigo4.vY);
        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(inimigoTexture);
    SDL_DestroyTexture(personagemTexture);
    free(event);

    return tempoRestante;
}

//pikachu
int faseTematica8(){
    //tematica8
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/pokemon.jpg");
    SDL_Texture* personagemTexture = carregarImagem("./media/skins/player/pikachu.png");
    SDL_Texture* inimigoTexture = carregarImagem("./media/skins/enemy/pokebola.png");

    //MUSICA
    play_Music("./media/songs/Pokemon_Theme.ogg");

    //TEMPO
    int tempoRestante = 20; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempoRestante, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem:
    SDL_Rect personagem = {500, 550, 85, 85}; //personagem
    Var varPersonagem = {0,0,0,0,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1: flutuante
    SDL_Rect inimigo1 = {500, 300, 50, 50}; //personagem
    Var varInimigo1 = {5,-5,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy2: flutuante
    SDL_Rect inimigo2 = {700, 400, 50, 50}; //personagem
    Var varInimigo2 = {5,4,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy3: flutuante
    SDL_Rect inimigo3 = {400, 200, 50, 50}; //personagem
    Var varInimigo3 = {4,-5,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy4: flutuante
    SDL_Rect inimigo4 = {200, 200, 50, 50}; //personagem
    Var varInimigo4 = {3,-5,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy5: flutuante
    SDL_Rect inimigo5 = {400, 200, 50, 50}; //personagem
    Var varInimigo5 = {2,-5,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    //enemy4: flutuante
    SDL_Rect inimigo6 = {200, 200, 50, 50}; //personagem
    Var varInimigo6 = {5,-2,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------


    while(tempoRestante >= 0 && running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem);
        }

        //OBJETOS:
        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagem:
        SDL_RenderCopy(gRenderer, personagemTexture, 0, &personagem);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo1);

        //print inimigo2:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo2);

        //print inimigo3:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo3);
        //-------------------

        //print inimigo4:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo4);
        //-------------------

        //print inimigo5:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo5);
        //-------------------

        //print inimigo6:
        SDL_RenderCopy(gRenderer, inimigoTexture, 0, &inimigo6);
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem, &inimigo1)||coll2Circles(&personagem, &inimigo2)||coll2Circles(&personagem, &inimigo3)||coll2Circles(&personagem, &inimigo4)||coll2Circles(&personagem, &inimigo5)||coll2Circles(&personagem, &inimigo6)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem, &varPersonagem);
        doTheCtrl(&ctrlPersonagem, &varPersonagem, &handPersonagem, contWhile);
        velObj(&personagem.x, &personagem.y, &varPersonagem.vX, &varPersonagem.vY);

        //inimigo1: flutuante
        collRectWall(&inimigo1, &varInimigo1);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);
        //-----------------------

        //inimigo2: flutuante
        collRectWall(&inimigo2, &varInimigo2);
        velObj(&inimigo2.x, &inimigo2.y, &varInimigo2.vX, &varInimigo2.vY);
        //-----------------------

        //inimigo3: flutuante
        collRectWall(&inimigo3, &varInimigo3);
        velObj(&inimigo3.x, &inimigo3.y, &varInimigo3.vX, &varInimigo3.vY);
        //-----------------------

        //inimigo4: flutuante
        collRectWall(&inimigo4, &varInimigo4);
        velObj(&inimigo4.x, &inimigo4.y, &varInimigo4.vX, &varInimigo4.vY);
        //-----------------------

        //inimigo5: flutuante
        collRectWall(&inimigo5, &varInimigo5);
        velObj(&inimigo5.x, &inimigo5.y, &varInimigo5.vX, &varInimigo5.vY);
        //-----------------------

        //inimigo6: flutuante
        collRectWall(&inimigo6, &varInimigo6);
        velObj(&inimigo6.x, &inimigo6.y, &varInimigo6.vX, &varInimigo6.vY);
        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempoRestante, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(inimigoTexture);
    SDL_DestroyTexture(personagemTexture);
    free(event);

    return tempoRestante;
}

int faseMulti(){
    //declarem td aqui em cima:
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true; //mantem o laço principal rodando
    int contWhile = 1; //sempre contWhile = 1;
    int tantoFaz = 0; //use para parametros q tanto fazem

    //TEXTURAS
    SDL_Texture* fundoTexture = carregarImagem("./media/backgrounds/cidade_batman.png");
    SDL_Texture* personagem1Texture = carregarImagem("./media/skins/player/batman.png");
    SDL_Texture* personagem2Texture = carregarImagem("./media/skins/player/superman.png");
    SDL_Texture* inimigo1Texture = carregarImagem("./media/skins/enemy/apocalipse.png");
    SDL_Texture* inimigo2Texture = carregarImagem("./media/skins/enemy/coringa.png");

    //MUSICA
    play_Music("./media/songs/Ain't_It_Fun.ogg");

    //TEMPO
    int tempo = 0; //duração da fase em segundos
    int contador = SDL_GetTicks(); //registra o tempo atual no contador
    SDL_Rect tempoRect = {tantoFaz, 200, tantoFaz, tantoFaz}; //contem a tempoTexture (onde mostra o tempo).
    SDL_Color tempoColor = { 255, 0, 0, 255}; //cor do texto com o tempo
    // carrega a primeira textura do tempo
    SDL_Texture* tempoTexture = criarTexture(tempo, tempoColor, &tempoRect);
    //-------------------------

    //OBJETOS:

    //personagem1:
    SDL_Rect personagem1 = {500, 550, 85, 85}; //personagem
    Var varPersonagem1 = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem1 = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem1 = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //personagem2:
    SDL_Rect personagem2 = {300, 550, 85, 85}; //personagem
    Var varPersonagem2 = {0,0,0,1,3}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Hand handPersonagem2 = {1,1,5}; //struct de controle do objeto (handlingX, handlingY, freqCtrl); "handling == dirigibilidade"
    Ctrl ctrlPersonagem2 = {false, false, false, false}; //struct bool direçoes de controle
    //-----------------------------------

    //enemy1:
    SDL_Rect inimigo1 = {0, 0, 80, 80}; //personagem
    Var varInimigo1 = {0,7,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo1 = {0, 0, SCREEN_WIDTH - inimigo1.w, SCREEN_HEIGHT - inimigo1.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy2:
    SDL_Rect inimigo2 = {SCREEN_WIDTH - 80, SCREEN_HEIGHT - 80, 80, 80}; //personagem
    Var varInimigo2 = {0,-7,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    Rota rotaInimigo2 = {0, 0, SCREEN_WIDTH - inimigo2.w, SCREEN_HEIGHT - inimigo2.h}; //(a rota eh um retangulo de onde objeto nao consegue sair)
    //-----------------------------------

    //enemy3:
    SDL_Rect inimigo3 = {700, 200, 50, 50}; //personagem
    Var varInimigo3 = {5,-5,tantoFaz,tantoFaz,tantoFaz}; //variação do objeto (velocidadeX, velocidadeY, aceleraçãoX, aceleraçãoY, freqAcel)
    //-----------------------------------

    while(running){
        //verifica os eventos:
        while(SDL_PollEvent(event)){
            //fechar janela
            closeWindow(event, &running);
            //pega os comandos para o personagem:
            ctrlObj(event, &ctrlPersonagem1);
            //pega os comandos para o personagem 2:
            ctrlExtra(event, &ctrlPersonagem2);
        }

        //OBJETOS:
        // imprime o fundo:
        SDL_RenderCopy(gRenderer, fundoTexture, NULL, NULL);

        //tempo restante:
        SDL_RenderCopy(gRenderer, tempoTexture, 0, &tempoRect);

        //OBJETOS:
        //print personagens:
        SDL_RenderCopy(gRenderer, personagem1Texture, 0, &personagem1);
        SDL_RenderCopy(gRenderer, personagem2Texture, 0, &personagem2);

        //print inimigo1:
        SDL_RenderCopy(gRenderer, inimigo2Texture, 0, &inimigo1);

        //print inimigo2:
        SDL_RenderCopy(gRenderer, inimigo2Texture, 0, &inimigo2);

        //print inimigo3:
        SDL_RenderCopy(gRenderer, inimigo1Texture, 0, &inimigo3);
        //-------------------

        //atualiza a tela:
        SDL_RenderPresent(gRenderer);
        //-------------------------------------------------------- aqui a tela é atualizada

        //INTERACOES FISICAS

        //verifica se duas circunferências se intersectam:
        if(coll2Circles(&personagem1, &inimigo1)||coll2Circles(&personagem1, &inimigo2)||coll2Circles(&personagem1, &inimigo3)||
                coll2Circles(&personagem2, &inimigo1)||coll2Circles(&personagem2, &inimigo2)||coll2Circles(&personagem2, &inimigo3)) {
            running = false;
        }

        //personagem:
        collRectWall(&personagem1, &varPersonagem1);
        doTheCtrl(&ctrlPersonagem1, &varPersonagem1, &handPersonagem1, contWhile);
        acelObj(&varPersonagem1, contWhile);
        velObj(&personagem1.x, &personagem1.y, &varPersonagem1.vX, &varPersonagem1.vY);

        // personagem 2
        collRectWall(&personagem2, &varPersonagem2);
        doTheCtrl(&ctrlPersonagem2, &varPersonagem2, &handPersonagem2, contWhile);
        acelObj(&varPersonagem2, contWhile);
        velObj(&personagem2.x, &personagem2.y, &varPersonagem2.vX, &varPersonagem2.vY);

        //inimigo1:
        collEnemyWall(&inimigo1, &rotaInimigo1, &varInimigo1);
        velObj(&inimigo1.x, &inimigo1.y, &varInimigo1.vX, &varInimigo1.vY);

        //inimigo2:
        collEnemyWall(&inimigo2, &rotaInimigo2, &varInimigo2);
        velObj(&inimigo2.x, &inimigo2.y, &varInimigo2.vX, &varInimigo2.vY);

        //inimigo3:
        collRectWall(&inimigo3, &varInimigo3);
        velObj(&inimigo3.x, &inimigo3.y, &varInimigo3.vX, &varInimigo3.vY);
        //-----------------------

        //verifica se passou um segundo desde o ultimo registro do tempo:
        if (SDL_GetTicks() - contador >= 1000) {
            // gambiarra:
            tempo += 2;
            //atualiza o tempo, destroi a tempoTexture, atualiza o contador e retorna uma texture com o tempo atualizado:
            tempoTexture = atualizaTempoMostrado(&tempo, tempoTexture, &contador, &tempoColor, &tempoRect);
        }

        //atualiza o contWhile
        atualizaContWhile(&contWhile, 16); //O segundo argumento recebe o (MMC(todos os freqAcel e freqCtrl) + 1)
        //delay pra controlar a velocidade de atualização da tela:
        SDL_Delay(20);
    }
    //-----------------
    SDL_DestroyTexture(tempoTexture);
    SDL_DestroyTexture(fundoTexture);
    SDL_DestroyTexture(personagem1Texture);
    SDL_DestroyTexture(personagem2Texture);
    SDL_DestroyTexture(inimigo1Texture);
    SDL_DestroyTexture(inimigo2Texture);
    free(event);
}