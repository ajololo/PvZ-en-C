#ifndef VISUALS
#define VISUALS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

SDL_Texture *cargarImagen(SDL_Renderer*, char*);

void movimientos(SDL_Event, int*, int*);

int Visualizacion_y_Audio(int **);

#endif
