#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include "logica.h"
#include "visuals.h"


int main() {
    int **Jardin = malloc(5*sizeof(int*));
    for(int i = 0; i < 5; i++) {
        Jardin[i] = calloc(8,sizeof(int));
    }

    for(int i = 0; i < 5; i++) {
        Jardin[i][0] = 1;
        Jardin[i][1] = 2;
        Jardin[i][7] = 10;
    }

    int ejecutando = Visualizacion_y_Audio(Jardin);
    if(ejecutando == 0)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
