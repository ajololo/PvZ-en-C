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

int menu();

enum matriz {
    PASTO = 0,
    GIRASOL = 1,
    LANZAG = 2,
    NUEZ = 3,
    PIMENTON = 4,
    GUISANTE = 5,
    ZOMBIE = 10,
    FLAG_ZOMBIE = 11,
    SOL = 20
};

#endif
