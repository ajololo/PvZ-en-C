#include "Tarea1.h"
#include "Tarea2.h"

int main(int argc, char **argv) {

    // Comprobar que se recibieron los dos nombres de archivo
    if (argc != 3) {
        printf("Uso: %s Config.txt Mapa.txt\n", argv[0]);
        return 1;
    }

    // Crear las estructuras donde guardaremos los datos
    Config config;
    Mapa mapa;
    EstadoJuego estado;

    //lee el archivo de configuracion y si no se puede abri correctamente da 1
    if (leer_config(argv[1], &config) != 0) {
    printf("No se pudo cargar la configuracion\n");
    return 1;
    }
    int soles = config.soles_iniciales;
    int vida_casa = config.vida_casa;
    //lee el archivo del mapa y si no se puede abri correctamente da 1
    if (leer_mapa(argv[2], &mapa) != 0) {
        printf("No se pudo cargar el mapa\n");
        return 1;
    }
    inicializar_vidas(&mapa, &config);
    inicializar_estado_juego(&estado, &config);
    //mostrar algunos datos de la configuraciónen la consola
    printf("configuracion cargada correctamente:\n");
    printf("oleadas: %d\n", config.oleadas_max);
    printf("vida de la casa: %d\n", config.vida_casa);
    printf("vida del zombie: %d\n", config.vida_zombie);
    printf("costo del girasol: %d\n", config.costo_girasol);

    //mostrar las dimensiones del mapa en la consola
    printf("\nMapa cargado correctamente:\n");
    printf("Filas: %d\n", mapa.filas);
    printf("Columnas: %d\n", mapa.columnas);

    //mostrar el contenido de la matriz en la consola
    for (int i = 0; i < mapa.filas; i++) {
        for (int j = 0; j < mapa.columnas; j++) {
            printf("%d ", mapa.celdas[i][j]);
        }
        printf("\n");
    }
while (estado.fase != TERMINADO) {
    actualizar_juego(&mapa, &config, &estado, &soles, &vida_casa);
}

    //resultado del juego
    if (vida_casa <= 0) {
        printf("Derrota! los zombies ganaron!\n");
    }
    else {
        printf("Ganaste! terminaron las oleadas\n");
    }

    liberar_mapa(&mapa);
    return 0;
}
