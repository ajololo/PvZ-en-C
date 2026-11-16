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
        Jardin[i][3] = 5;
        Jardin[i][7] = 10;
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
