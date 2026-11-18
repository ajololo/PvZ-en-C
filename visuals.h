#ifndef VISUALS
#define VISUALS

#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_audio.h>

#define TAM_PIX 128
#define FILAS 5
#define COLUMNAS 8

SDL_Texture *cargarImagen(SDL_Renderer*, char*);

void movimientos(SDL_Event, int*, int*);

int Visualizacion_y_Audio(int **);

#endif
