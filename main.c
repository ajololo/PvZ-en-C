#include "visuals.h"

int main() {
    int ini_juego = menu();
    if(ini_juego == 1)
        return EXIT_FAILURE;
    else if(ini_juego == 2)
        return EXIT_SUCCESS;
    
    int **Jardin = malloc(5*sizeof(int*));
    for(int i = 0; i < 5; i++) {
        Jardin[i] = calloc(8,sizeof(int));
    }

    for(int i = 0; i < 5; i++) {
        Jardin[i][0] = 1;
        Jardin[i][1] = 2;
        Jardin[i][3] = 4;
        Jardin[i][7] = 11;
    }

    int ejecutando = Visualizacion_y_Audio(Jardin);
    if(ejecutando == 0) {
        for(int i = 0; i < 5; i++) {
            free(Jardin[i]);
        }
        free(Jardin);
        return EXIT_SUCCESS;
    }
    else {
        for(int i = 0; i < 5; i++) {
            free(Jardin[i]);
        }
        free(Jardin);
        return EXIT_FAILURE;
    }
}
