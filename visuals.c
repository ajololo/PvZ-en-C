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

    SDL_Texture *spr_girasol = cargarImagen(render, "Imágenes/girasol.png");
    SDL_Texture *spr_lanzag = cargarImagen(render, "Imágenes/lanzag.png");
    SDL_Texture *spr_nuez = cargarImagen(render, "Imágenes/nuez.png");
    SDL_Texture *spr_zombie = cargarImagen(render, "Imágenes/zombie.png");
    SDL_Texture *spr_jugador = cargarImagen(render, "Imágenes/jugador.png");
    SDL_Texture *spr_guisante = cargarImagen(render, "Imágenes/guisante.png");
    SDL_Texture *spr_prev_girasol = cargarImagen(render, "Imágenes/p_girasol.png");
    SDL_Texture *spr_prev_lanzag = cargarImagen(render, "Imágenes/p_lanzag.png");
    SDL_Texture *spr_prev_nuez = cargarImagen(render, "Imágenes/p_nuez.png");

    if(!spr_girasol || !spr_lanzag || !spr_nuez || !spr_zombie || !spr_jugador || !spr_guisante || !spr_prev_girasol || !spr_prev_lanzag || !spr_prev_nuez) {
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Uint8 *buf = NULL;
    Uint32 len = 0;
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
        // Mantiene la música reproduciéndose en bucle
        if(SDL_GetAudioStreamQueued(stream) < len)
           SDL_PutAudioStreamData(stream, buf, len);

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT)
                estado = 0;
            movimientos(event, &pos_x, &pos_y);

            SDL_SetRenderDrawColor(render, 20, 100, 30, 255);
            SDL_RenderClear(render);
            for(int i = 0; i < FILAS; i++) {
                for(int j = 0; j < COLUMNAS; j++) {
                    if(A[i][j] == 1) {
                        SDL_FRect rGirasol = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_girasol, NULL, &rGirasol);
                    }
                    else if(A[i][j] == 2) {
                        SDL_FRect rLanzag = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_lanzag, NULL, &rLanzag);
                    }
                    else if(A[i][j] == 3) {
                        SDL_FRect rNuez = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_nuez, NULL, &rNuez);
                    }
                    else if(A[i][j] == 5) {
                        SDL_FRect rGuisante = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_guisante, NULL, &rGuisante);
                    }
                    else if(A[i][j] == 10) {
                        SDL_FRect rZombie = {j * TAM_PIX, i * TAM_PIX, TAM_PIX, TAM_PIX};
                        SDL_RenderTexture(render, spr_zombie, NULL, &rZombie);
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
            }
            if(planta_prev == 0)
                SDL_RenderTexture(render, spr_jugador, NULL, &rJugador);
            else if(planta_prev == 1)
                SDL_RenderTexture(render, spr_prev_girasol, NULL, &rJugador);
            else if(planta_prev == 2)
                SDL_RenderTexture(render, spr_prev_lanzag, NULL, &rJugador);
            else
                SDL_RenderTexture(render, spr_prev_nuez, NULL, &rJugador);
            
            SDL_RenderPresent(render);
            SDL_Delay(16);
        }
    }

    SDL_free(buf);
    SDL_DestroyAudioStream(stream);
    SDL_DestroyTexture(spr_jugador);
    SDL_DestroyTexture(spr_girasol);
    SDL_DestroyTexture(spr_zombie);
    SDL_DestroyTexture(spr_lanzag);
    SDL_DestroyTexture(spr_nuez);
    SDL_DestroyTexture(spr_guisante);
    SDL_DestroyTexture(spr_prev_lanzag);
    SDL_DestroyTexture(spr_prev_girasol);
    SDL_DestroyTexture(spr_prev_nuez);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}