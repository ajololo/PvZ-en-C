#ifndef TAREA1_H
#define TAREA1_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int filas;
    int columnas;
    int **celdas;
    int **vidas;
}Mapa;

typedef struct {
    //general
    int oleadas_max;
    //hp
    int vida_casa;
    int vida_zombie;
    int vida_Flag_zombie;

    int vida_girasol;
    int vida_lanzaguisantes;
    int vida_nuez;
    //economia
    int costo_girasol;
    int costo_lanzaguisantes;
    int costo_nuez;
    int costo_pimenton;

    int soles_iniciales;
    int soles_por_girasol;
    int soles_por_generacion;
    //frecuencia y velocidad
    int frecuencia_generacion_zombie;
    int frecuencia_soles_base;
    int frecuencia_soles_girasol;
    int frecuencia_disparo;
    
    int velocidad_zombie;
    int velocidad_flag_zombie;
    //tiempo
    int tiempo_preparacion;
    int tiempo_descanso;
} Config;

int leer_mapa(const char *nombre_archivo, Mapa *mapa);

void liberar_mapa(Mapa *mapa);

typedef struct {
    char nombre[50];
    int *valor;
} DatosCfg;

int leer_config(const char *nombre_archivo, Config *config);

enum matriz {
PASTO = 0,
GIRASOL = 1,
LANZAGUISANTES = 2,
NUEZ = 3,
PIMENTON = 4,
ZOMBIE = 5,
FLAG_ZOMBIE = 6
};

enum fase_juego {
    PREPARACION,
    OLEADA,
    DESCANSO,
    TERMINADO
};

#endif 