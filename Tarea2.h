#ifndef TAREA2_H
#define TAREA2_H

#include "Tarea1.h"
#include <time.h>

//estado general del juego
typedef struct {
    enum fase_juego fase;
    int oleada_actual;
    int tiempo_restante;

    time_t ultimo_tick;

    //zombis que esperan aparecer
    int zombies_basicos_pendientes;
    int zombies_bandera_pendientes;
    time_t ultima_generacion_zombie;

    //tiempo de acciones
    time_t ultimo_movimiento_zombie;
    time_t ultimo_movimiento_flag;
    time_t ultimo_disparo;
    time_t ultima_generacion_soles;
    time_t ultimo_danio_casa;
    time_t ultima_generacion_soles_girasol;
} EstadoJuego;

//funciones
void mover_zombies(Mapa *mapa, Config *config, EstadoJuego *estado);
void inicializar_vidas(Mapa *mapa, Config *config);
void disparar_lanzaguisantes(Mapa *mapa, Config *config, EstadoJuego *estado);
int zombi_en_casa(Mapa *mapa);
int colocar_planta(Mapa *mapa, Config *config, int *soles, int tipo_planta, int fila, int columna, EstadoJuego *estado);
void inicializar_estado_juego(EstadoJuego *estado, Config *config);
void actualizar_tiempo(EstadoJuego *estado);
void finalizar_oleada(EstadoJuego *estado, Config *config);
int quedan_zombies(Mapa *mapa);
int posiciones_disponibles(Mapa *mapa);
void preparar_oleada(EstadoJuego *estado);
void generar_zombie_intervalo(Mapa *mapa, Config *config, EstadoJuego *estado);
void generar_soles(Mapa *mapa, Config *config, int *soles, EstadoJuego *estado);
void actualizar_juego(Mapa *mapa, Config *config, EstadoJuego *estado, int *soles, int *vida_casa);
#endif
