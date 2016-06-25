void historia(){

    // variaveis
    SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
    bool running = true;

    // cria vetores de texture e rect para todas imagens
    SDL_Texture* textures[ultima];
    SDL_Rect rects[ultima];
    bool hover[ultima];

    int i;
    char nome[30];
    for (i = 0; i < ultima; i++) {
        sprintf(nome, "./media/thumbs/fase%d.png", i+1);
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
                for (i = 0; i < ultima; i++) {
                    if (collRectPoint(&rects[i], x, y)) {
                        //ABRE fase
                        int valor = faseNumero(i);
                        if (valor < 0) {
                            salvar(valor);
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
}