#include "visuals.h"

SDL_Texture *cargarImagen(SDL_Renderer *render, char *nombre_archivo) {
    SDL_Texture *sprite = IMG_LoadTexture(render, nombre_archivo);

    if(sprite == NULL)
        SDL_Log("Error: %s. No se pudo cargar %s", SDL_GetError(), nombre_archivo);
    
    return sprite;
}

void movimientos(SDL_Event event, int *x, int *y) {
    if(event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
        int Npos_x = *x;
        int Npos_y = *y;
        if(event.key.scancode == SDL_SCANCODE_UP)
            Npos_y--;
        else if(event.key.scancode == SDL_SCANCODE_RIGHT)
            Npos_x++;
        else if(event.key.scancode == SDL_SCANCODE_DOWN)
            Npos_y++;
        else if(event.key.scancode == SDL_SCANCODE_LEFT)
            Npos_x--;
        
        if(Npos_x >= 0 && Npos_x < COLUMNAS && Npos_y >= 0 && Npos_y < FILAS) {
            *x = Npos_x;
            *y = Npos_y;
        }
    }
}

int menu() {
    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;

    if(!SDL_Init(SDL_INIT_VIDEO) || !SDL_Init(SDL_INIT_AUDIO)) {
        SDL_Log("Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if(!SDL_CreateWindowAndRenderer("Menú", COLUMNAS * TAM_PIX, FILAS * TAM_PIX, 0, &window, &render)) {
        SDL_Log("Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture *spr_menu1 = cargarImagen(render, "Imágenes/menu1.png");
    SDL_Texture *spr_menu2 = cargarImagen(render, "Imágenes/menu2.png");

    if(!spr_menu1 || !spr_menu2) {
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    unsigned char *buf = NULL;
    unsigned int len = 0;
    SDL_AudioSpec specs; // Struct que guarda la frecuencia del audio, el peso, y demás características que se deben tomar en cuenta para que este se reproduzca y suene "bien"
    if(!SDL_LoadWAV("Sonidos/musica_menu.wav", &specs, &buf, &len)) {
        SDL_Log("Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_AudioStream *stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &specs, NULL, NULL); 
    if(stream == NULL) {
        SDL_Log("Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if(!SDL_PutAudioStreamData(stream, buf, len)) {
        SDL_Log("Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Al inicializar el stream de audio, este viene en pausa por defecto, por lo que debemos quitársela 
    SDL_ResumeAudioStreamDevice(stream);

    char estado = 1;
    while(estado == 1 || estado == 2) {
        // Mantiene la música reproduciéndose en bucle
        if(SDL_GetAudioStreamQueued(stream) < len)
           SDL_PutAudioStreamData(stream, buf, len);
        
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                SDL_free(buf);
                SDL_DestroyAudioStream(stream);
                SDL_DestroyTexture(spr_menu1);
                SDL_DestroyTexture(spr_menu2);
                SDL_DestroyRenderer(render);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 1;
            }

            if(event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
                if(event.key.scancode == SDL_SCANCODE_ESCAPE)
                    return 2;
                else if(event.key.scancode == SDL_SCANCODE_SPACE)
                    estado++;
            }
        }

        SDL_RenderClear(render);
        if(estado == 1) {
                SDL_FRect rMenu1 = {0, 0, COLUMNAS * TAM_PIX, FILAS * TAM_PIX};
                SDL_RenderTexture(render, spr_menu1, NULL, &rMenu1);
            }
            else if(estado == 2) {
                SDL_FRect rMenu2 = {0, 0, COLUMNAS * TAM_PIX, FILAS * TAM_PIX};
                SDL_RenderTexture(render, spr_menu2, NULL, &rMenu2);
            }

        SDL_RenderPresent(render);
        SDL_Delay(16);
    }

    SDL_free(buf);
    SDL_DestroyAudioStream(stream);
    SDL_DestroyTexture(spr_menu1);
    SDL_DestroyTexture(spr_menu2);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int Visualizacion_y_Audio(int **A) {
    int pos_x = 0, pos_y = 0, planta_prev = 0;
    char estado = 1; // Mientras esta variable sea uno, el juego se seguirá ejecutando

    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;

    if(!SDL_Init(SDL_INIT_VIDEO) || !SDL_Init(SDL_INIT_AUDIO)) {
        SDL_Log("Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if(!SDL_CreateWindowAndRenderer("Jardín", COLUMNAS * TAM_PIX, FILAS * TAM_PIX, 0, &window, &render)) {
        SDL_Log("Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture *spr_cesped1 = cargarImagen(render, "Imágenes/cesped1.png");
    SDL_Texture *spr_cesped2 = cargarImagen(render, "Imágenes/cesped2.png");
    SDL_Texture *spr_girasol = cargarImagen(render, "Imágenes/girasol.png");
    SDL_Texture *spr_lanzag = cargarImagen(render, "Imágenes/lanzag.png");
    SDL_Texture *spr_nuez = cargarImagen(render, "Imágenes/nuez.png");
    SDL_Texture *spr_pimenton = cargarImagen(render, "Imágenes/chile.png");
    SDL_Texture *spr_zombie = cargarImagen(render, "Imágenes/zombie.png");
    SDL_Texture *spr_zombie_flag = cargarImagen(render, "Imágenes/zombie_flag.png");
    SDL_Texture *spr_jugador = cargarImagen(render, "Imágenes/jugador.png");
    SDL_Texture *spr_guisante = cargarImagen(render, "Imágenes/guisante.png");
    SDL_Texture *spr_sol = cargarImagen(render, "Imágenes/sol.png");
    SDL_Texture *spr_prev_girasol = cargarImagen(render, "Imágenes/p_girasol.png");
    SDL_Texture *spr_prev_lanzag = cargarImagen(render, "Imágenes/p_lanzag.png");
    SDL_Texture *spr_prev_nuez = cargarImagen(render, "Imágenes/p_nuez.png");
    SDL_Texture *spr_prev_pimenton = cargarImagen(render, "Imágenes/p_chile.png");

    if(!spr_cesped1 || !spr_cesped2 || !spr_girasol || !spr_lanzag || !spr_nuez || !spr_pimenton || 
        !spr_zombie || !spr_zombie_flag || !spr_jugador || !spr_guisante || !spr_sol || !spr_prev_girasol || 
        !spr_prev_lanzag || !spr_prev_nuez || !spr_prev_pimenton) {
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    unsigned char *buf = NULL;
    unsigned int len = 0;
    SDL_AudioSpec specs;
    if(!SDL_LoadWAV("Sonidos/musica.wav", &specs, &buf, &len)) {
        SDL_Log("Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_AudioStream *stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &specs, NULL, NULL); 
    if(stream == NULL) {
        SDL_Log("Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if(!SDL_PutAudioStreamData(stream, buf, len)) {
        SDL_Log("Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_ResumeAudioStreamDevice(stream);

    while(estado) {
        if(SDL_GetAudioStreamQueued(stream) < len)
           SDL_PutAudioStreamData(stream, buf, len);

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT)
                estado = 0;

            SDL_RenderClear(render);
            movimientos(event, &pos_x, &pos_y);

            // Renderizamos las imágenes del césped de fondo
            for(int i = 0; i < FILAS; i++) {
                for(int j = 0; j < COLUMNAS; j++) {
                    if(j % 2 == 0) {
                        SDL_FRect rCesped1 = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_cesped1, NULL, &rCesped1);
                    }
                    else {
                        SDL_FRect rCesped2 = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_cesped2, NULL, &rCesped2);
                    }
                }
            }
            
            // Renderizamos las imágenes que la matriz indique
            for(int i = 0; i < FILAS; i++) {
                for(int j = 0; j < COLUMNAS; j++) {
                    if(A[i][j] == GIRASOL) {
                        SDL_FRect rGirasol = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_girasol, NULL, &rGirasol);
                    }
                    else if(A[i][j] == LANZAG) {
                        SDL_FRect rLanzag = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_lanzag, NULL, &rLanzag);
                    }
                    else if(A[i][j] == NUEZ) {
                        SDL_FRect rNuez = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_nuez, NULL, &rNuez);
                    }
                    else if(A[i][j] == PIMENTON) {
                        SDL_FRect rPimenton = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_pimenton, NULL, &rPimenton);
                    }
                    else if(A[i][j] == GUISANTE) {
                        SDL_FRect rGuisante = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_guisante, NULL, &rGuisante);
                    }
                    else if(A[i][j] == ZOMBIE) {
                        SDL_FRect rZombie = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_zombie, NULL, &rZombie);
                    }
                    else if(A[i][j] == FLAG_ZOMBIE) {
                        SDL_FRect rZombieF = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_zombie_flag, NULL, &rZombieF);
                    }
                    else if(A[i][j] == SOL) {
                        SDL_FRect rSol = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_sol, NULL, &rSol);
                    }
                }
            }

            SDL_FRect rJugador = {pos_x * TAM_PIX, pos_y * TAM_PIX, TAM_PIX, TAM_PIX};

            // Al presionar Z, X, o C, podemos previsualizar qué planta podemos colocar en la casilla seleccionada
            if(event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
                if(event.key.scancode == SDL_SCANCODE_Z) {
                    if(planta_prev != 1)
                        planta_prev = 1;
                    else
                        planta_prev = 0;
                }
                else if(event.key.scancode == SDL_SCANCODE_X) {
                    if(planta_prev != 2)
                        planta_prev = 2;
                    else
                        planta_prev = 0;
                }
                else if(event.key.scancode == SDL_SCANCODE_C) {
                    if(planta_prev != 3)
                        planta_prev = 3;
                    else
                        planta_prev = 0;
                }
                else if(event.key.scancode == SDL_SCANCODE_V) {
                    if(planta_prev != 4)
                        planta_prev = 4;
                    else
                        planta_prev = 0;
                }
            }
            if(planta_prev == 0)
                SDL_RenderTexture(render, spr_jugador, NULL, &rJugador);
            else if(planta_prev == 1)
                SDL_RenderTexture(render, spr_prev_girasol, NULL, &rJugador);
            else if(planta_prev == 2)
                SDL_RenderTexture(render, spr_prev_lanzag, NULL, &rJugador);
            else if(planta_prev == 3)
                SDL_RenderTexture(render, spr_prev_nuez, NULL, &rJugador);
            else
                SDL_RenderTexture(render, spr_prev_pimenton, NULL, &rJugador);
            
            SDL_RenderPresent(render);
            SDL_Delay(16);
        }
    }

    SDL_free(buf);
    SDL_DestroyAudioStream(stream);
    SDL_DestroyTexture(spr_cesped1);
    SDL_DestroyTexture(spr_cesped2);
    SDL_DestroyTexture(spr_jugador);
    SDL_DestroyTexture(spr_girasol);
    SDL_DestroyTexture(spr_zombie);
    SDL_DestroyTexture(spr_zombie_flag);
    SDL_DestroyTexture(spr_lanzag);
    SDL_DestroyTexture(spr_nuez);
    SDL_DestroyTexture(spr_pimenton);
    SDL_DestroyTexture(spr_guisante);
    SDL_DestroyTexture(spr_prev_lanzag);
    SDL_DestroyTexture(spr_prev_girasol);
    SDL_DestroyTexture(spr_prev_nuez);
    SDL_DestroyTexture(spr_prev_pimenton);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}