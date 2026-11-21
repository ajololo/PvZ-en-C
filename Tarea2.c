#include "Tarea2.h"
//se le asigna las vidas a cada planta y zombi
void inicializar_vidas(Mapa *mapa, Config *config) {

    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            if (mapa->celdas[i][j] == ZOMBIE) {
                mapa->vidas[i][j] = config->vida_zombie;
                }
            else if (mapa->celdas[i][j] == FLAG_ZOMBIE) {
                mapa->vidas[i][j] = config->vida_Flag_zombie;
                }
            else if (mapa->celdas[i][j] == GIRASOL) {
                mapa->vidas[i][j] = config->vida_girasol;
                }
            else if (mapa->celdas[i][j] == LANZAGUISANTES) {
                mapa->vidas[i][j] = config->vida_lanzaguisantes;
                }
            else if (mapa->celdas[i][j] == NUEZ) {
                mapa->vidas[i][j] = config->vida_nuez;
                }
            }
        }
    }
void mover_zombies(Mapa *mapa, Config *config, EstadoJuego *estado) {
    time_t ahora = time(NULL);
    int mover_zombie = difftime(ahora, estado->ultimo_movimiento_zombie)
                       >= config->velocidad_zombie;
    int mover_flag = difftime(ahora, estado->ultimo_movimiento_flag)
                     >= config->velocidad_flag_zombie;
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            int tipo = mapa->celdas[i][j];
            if ((tipo == ZOMBIE && mover_zombie) ||
                (tipo == FLAG_ZOMBIE && mover_flag)) {
                if (j > 0) {
                    if (mapa->celdas[i][j - 1] == PASTO) {
                        mapa->celdas[i][j - 1] = tipo;
                        mapa->vidas[i][j - 1] = mapa->vidas[i][j];
                        mapa->celdas[i][j] = PASTO;
                        mapa->vidas[i][j] = 0;
                    }
                    else if (mapa->celdas[i][j - 1] == GIRASOL ||
                             mapa->celdas[i][j - 1] == LANZAGUISANTES ||
                             mapa->celdas[i][j - 1] == NUEZ) {
                        mapa->vidas[i][j - 1]--;
                        if (mapa->vidas[i][j - 1] <= 0) {
                            mapa->celdas[i][j - 1] = PASTO;
                            mapa->vidas[i][j - 1] = 0;
                        }
                    }
                }
            }
        }
    }

    if (mover_zombie) {
        estado->ultimo_movimiento_zombie = ahora;
    }

    if (mover_flag) {
        estado->ultimo_movimiento_flag = ahora;
    }
}
void disparar_lanzaguisantes(Mapa *mapa, Config *config, EstadoJuego *estado) {

    time_t ahora = time(NULL);
    if (difftime(ahora, estado->ultimo_disparo) < config->frecuencia_disparo) {
        return;
    }
    estado->ultimo_disparo = ahora;
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            if (mapa->celdas[i][j] == LANZAGUISANTES) {
                for (int k = j + 1; k < mapa->columnas; k++) {
                    if (mapa->celdas[i][k] == ZOMBIE ||
                        mapa->celdas[i][k] == FLAG_ZOMBIE) {
                        mapa->vidas[i][k]--;
                        if (mapa->vidas[i][k] <= 0) {
                            mapa->celdas[i][k] = PASTO;
                            mapa->vidas[i][k] = 0;
                        }
                        break;
                    }
                }
            }
        }
    }
}
int zombi_en_casa(Mapa *mapa) {
    for (int i = 0; i < mapa->filas; i++) {
        if (mapa->celdas[i][0] == ZOMBIE ||
            mapa->celdas[i][0] == FLAG_ZOMBIE) {
                return 1;
            }
        }
        return 0; 
    }
    
int colocar_planta(Mapa *mapa, Config *config, int *soles, int tipo_planta, int fila, int columna, EstadoJuego *estado) {
    if (estado->fase == TERMINADO) {
    return 0;
    }
    //se ve si existe esa posicion
    if (fila < 0 || fila >= mapa->filas || columna < 0 || columna >= mapa->columnas) {
        return 0;
    }
    //se ve si es pasto para poder poner algo
    if (mapa->celdas[fila][columna] != PASTO) {
        return 0;
    }
    //valor planta
    int costo;
    if (tipo_planta == GIRASOL) {
        costo = config->costo_girasol;
    }
    else if (tipo_planta == LANZAGUISANTES) {
        costo = config->costo_lanzaguisantes;
    }
    else if (tipo_planta == NUEZ) {
        costo = config->costo_nuez;
    }
    else if (tipo_planta == PIMENTON) {
        costo = config->costo_pimenton;
    }
    else {
        return 0;
    }
    if (*soles < costo) {
        return 0;
    }
    *soles -=costo;
    mapa->celdas[fila][columna] = tipo_planta;
    if (tipo_planta == GIRASOL) {
        mapa->vidas[fila][columna] = config->vida_girasol;
    }
    else if (tipo_planta == LANZAGUISANTES) {
        mapa->vidas[fila][columna] = config->vida_lanzaguisantes;
    }
    else if (tipo_planta == NUEZ) {
        mapa->vidas[fila][columna] = config->vida_nuez;
    }
if (tipo_planta == PIMENTON) {
    for (int j = 0; j < mapa->columnas; j++) {
        if (mapa->celdas[fila][j] == ZOMBIE ||
            mapa->celdas[fila][j] == FLAG_ZOMBIE) {
            mapa->celdas[fila][j] = PASTO;
            mapa->vidas[fila][j] = 0;
        }
    }
    //el pimenton es de un solo yuso
    mapa->celdas[fila][columna] = PASTO;
    mapa->vidas[fila][columna] = 0;
}
return 1;
}
void inicializar_estado_juego(EstadoJuego *estado, Config *config) {
    estado->fase = PREPARACION;
    estado->oleada_actual = 0;
    estado->tiempo_restante = config->tiempo_preparacion;
    estado->ultimo_tick = time(NULL);
    estado->ultimo_movimiento_zombie = time(NULL);
    estado->ultimo_movimiento_flag = time(NULL);
    estado->ultimo_disparo = time(NULL);
    estado->ultima_generacion_soles = time(NULL);
    estado->ultimo_danio_casa = time(NULL);
    estado->zombies_basicos_pendientes = 0;
    estado->zombies_bandera_pendientes = 0;
    estado->ultima_generacion_zombie = time(NULL);
    estado->ultima_generacion_soles_girasol = time(NULL);
}
void actualizar_tiempo(EstadoJuego *estado) {
    if (estado->fase != PREPARACION &&
        estado->fase != DESCANSO) {
        return;
    }
    time_t ahora = time(NULL);
    int segundos = (int)difftime(ahora, estado->ultimo_tick);
    if (segundos >= 1) {
        estado->tiempo_restante -= segundos;
        estado->ultimo_tick = ahora;
    }
    if (estado->tiempo_restante <= 0) {
    estado->tiempo_restante = 0;
    estado->fase = OLEADA;
    estado->oleada_actual++;
    estado->ultimo_tick = time(NULL);
    preparar_oleada(estado);
    }
}
void finalizar_oleada(EstadoJuego *estado, Config *config) {
    if (estado->oleada_actual >= config->oleadas_max) {
        estado->fase = TERMINADO;
        return;
    }
    estado->fase = DESCANSO;
    estado->tiempo_restante = config->tiempo_descanso;
    estado->ultimo_tick = time(NULL);
}
int quedan_zombies(Mapa *mapa) {
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = mapa->columnas - 1; j >= 0; j--) {
            //busca zombies en el mapa
            if (mapa->celdas[i][j] == ZOMBIE ||
                mapa->celdas[i][j] == FLAG_ZOMBIE) {
                return 1;
            }
        }
    }
    return 0;
}
int posiciones_disponibles(Mapa *mapa) {
    int columna = mapa->columnas - 1;
    int disponibles = 0;
    for (int i = 0; i < mapa->filas; i++) {
        if (mapa->celdas[i][columna] == PASTO) {
            disponibles++;
        }
    }
    return disponibles;
}
void preparar_oleada(EstadoJuego *estado) {
    if (estado->oleada_actual == 1) {
        estado->zombies_basicos_pendientes = 2;
        estado->zombies_bandera_pendientes = 0;
    }
    else if (estado->oleada_actual == 2) {
        estado->zombies_basicos_pendientes = 3;
        estado->zombies_bandera_pendientes = 1;
    }
    else if (estado->oleada_actual == 3) {
        estado->zombies_basicos_pendientes = 4;
        estado->zombies_bandera_pendientes = 1;
    }
    else if (estado->oleada_actual == 4) {
        estado->zombies_basicos_pendientes = 5;
        estado->zombies_bandera_pendientes = 2;
    }
    else if (estado->oleada_actual == 5) {
        estado->zombies_basicos_pendientes = 6;
        estado->zombies_bandera_pendientes = 2;
    }
    estado->ultima_generacion_zombie = time(NULL);
}

void actualizar_juego(Mapa *mapa, Config *config, EstadoJuego *estado,
                      int *soles, int *vida_casa) {
    actualizar_tiempo(estado);
    if (estado->fase == TERMINADO) {
        return;
    }
    generar_soles(mapa, config, soles, estado);
    if (estado->fase == OLEADA) {
        generar_zombie_intervalo(mapa, config, estado);
        mover_zombies(mapa, config, estado);
        disparar_lanzaguisantes(mapa, config, estado);
        if (zombi_en_casa(mapa) == 1) {
            time_t ahora = time(NULL);
            if (difftime(ahora, estado->ultimo_danio_casa) >= 1) {
                (*vida_casa)--;
                estado->ultimo_danio_casa = ahora;

                if (*vida_casa <= 0) {
                    *vida_casa = 0;
                    estado->fase = TERMINADO;
                    return;
                }
            }
        }
        if (estado->zombies_basicos_pendientes == 0 &&
            estado->zombies_bandera_pendientes == 0 &&
            quedan_zombies(mapa) == 0) {
            finalizar_oleada(estado, config);
        }
    }
}
void generar_soles(Mapa *mapa, Config *config, int *soles, EstadoJuego *estado) {
    time_t ahora = time(NULL);
    // Generacion de soles base
    if (difftime(ahora, estado->ultima_generacion_soles)
        >= config->frecuencia_soles_base) {
        *soles += config->soles_por_generacion;
        estado->ultima_generacion_soles = ahora;
    }
    // Generacion de soles de los girasoles
    if (difftime(ahora, estado->ultima_generacion_soles_girasol)
        >= config->frecuencia_soles_girasol) {
        for (int i = 0; i < mapa->filas; i++) {
            for (int j = 0; j < mapa->columnas; j++) {
                if (mapa->celdas[i][j] == GIRASOL) {
                    *soles += config->soles_por_girasol;
                }
            }
        }
        estado->ultima_generacion_soles_girasol = ahora;
    }
}
void generar_zombie_intervalo(Mapa *mapa, Config *config, EstadoJuego *estado) {
    time_t ahora = time(NULL);
    //comprobar si quedan zombis por generar
    if (estado->zombies_basicos_pendientes == 0 &&
        estado->zombies_bandera_pendientes == 0) {
        return;
    }
    //comprobar si ya pasó el intervalo
    if (difftime(ahora, estado->ultima_generacion_zombie)
        < config->frecuencia_generacion_zombie) {
        return;
    }
    int columna = mapa->columnas - 1;
    //buscar una posición libre en la última columna
    for (int i = 0; i < mapa->filas; i++) {
        if (mapa->celdas[i][columna] == PASTO) {
            if (estado->zombies_basicos_pendientes > 0) {
                mapa->celdas[i][columna] = ZOMBIE;
                mapa->vidas[i][columna] = config->vida_zombie;
                estado->zombies_basicos_pendientes--;
            }
            else {
                mapa->celdas[i][columna] = FLAG_ZOMBIE;
                mapa->vidas[i][columna] = config->vida_Flag_zombie;
                estado->zombies_bandera_pendientes--;
            }
            estado->ultima_generacion_zombie = ahora;
            return;
        }
    }
}