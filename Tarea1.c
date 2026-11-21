#include "Tarea1.h"

int leer_mapa(const char *nombre_archivo, Mapa *mapa) {
    //Abrir el archivo
    FILE *Map = fopen(nombre_archivo, "r");

    if (Map == NULL) {
        printf("No se pudo abrir el archivo\n");
        return 1;
    }
    //Leer las dimensiones
    int filas;
    int columnas;
    if (fscanf(Map, "%d %d", &filas, &columnas) != 2){
        printf("error al leer las dimensiones\n");
        fclose(Map);
        return 1;
    }
    //Comprobar que las dimensiones sean válidas
    if (filas<=0 || columnas<=0) {
        printf("error, dimensiones invalidas\n");
        fclose(Map);
        return 1;
    }
    //Reservar memoria para los punteros de las filas
    int **celdas= malloc(filas * sizeof(int*));
    
    if (celdas==NULL){
        printf("error al reservar la memoria\n");
        fclose(Map);
        return 1;
    }
    //Reservar memoria para cada fila
    for (int i=0; i<filas; i++) {
        celdas[i]=malloc(columnas * sizeof(int));
        if (celdas[i]==NULL) {
            printf("error al rservar alguna fila\n");
            //liberar las filas que si se reservaron
            for (int j=0; j<i; j++) {
                free(celdas[j]);
            }
        free(celdas);
        fclose(Map);
        return 1;   
        }
    }
    //leer los valores y guardarlos en la matriz
    for (int i=0; i<filas; i++) {
        for (int j=0; j<columnas; j++) {
            
            if (fscanf(Map, "%d", &celdas[i][j]) != 1) {
                printf("error, no se leyeron los valores correctamente\n");
            
                for (int k=0; k < filas; k++) {
                    free(celdas[k]);
                }
                free(celdas);
                fclose(Map);
                return 1;
            }
            if (celdas[i][j] < PASTO || celdas[i][j] > FLAG_ZOMBIE) {
            printf("error, valor desconocido en el mapa\n");
                    for (int k = 0; k < filas; k++) {
                        free(celdas[k]);
                }

            free(celdas);
            fclose(Map);
            return 1;}
        }

    }
    char extra;

    if (fscanf(Map, " %c", &extra) == 1) {
        printf("error, hay por lo menos un dato adicional en el Mapa.txt\n");
        for (int k = 0; k < filas; k++) {
            free(celdas[k]);
                    }

        free(celdas);
        fclose(Map);
        return 1;
     }
    // Reservar memoria para los punteros de las filas de vidas
    int **vidas = malloc(filas * sizeof(int *));
    
    if (vidas == NULL) {
        printf("Error al reservar memoria para vidas\n");
    
        for (int k = 0; k < filas; k++) {
            free(celdas[k]);
        }
    
        free(celdas);
        fclose(Map);
        return 1;
    }
// Reservar memoria para cada fila de vidas
for (int i = 0; i < filas; i++) {

    vidas[i] = malloc(columnas * sizeof(int));

    if (vidas[i] == NULL) {

        // Liberar las filas de vidas reservadas
        for (int j = 0; j < i; j++) {
            free(vidas[j]);
        }

        free(vidas);

        // Liberar la matriz de celdas
        for (int j = 0; j < filas; j++) {
            free(celdas[j]);
        }

        free(celdas);
        fclose(Map);

        return 1;
    }

    //inicializar fila con ceros
    for (int j = 0; j < columnas; j++) {
        vidas[i][j] = 0;
    }
}
    //guardar la información en la estructura Mapa
    mapa->filas = filas;
    mapa->columnas = columnas;
    mapa->celdas = celdas;
    mapa->vidas = vidas;

    fclose(Map);

    return 0;
}
    //por si es que se necesita liberar la memoria de nuestra matriz desde distintas partes
void liberar_mapa(Mapa *mapa) {

    for (int i = 0; i < mapa->filas; i++) {
        free(mapa->celdas[i]);
        free(mapa->vidas[i]);
    }

    free(mapa->celdas);
    free(mapa->vidas);

    mapa->celdas = NULL;
    mapa->vidas = NULL;
    
    mapa->filas = 0;
    mapa->columnas = 0;
}

int leer_config(const char *nombre_archivo, Config *config) {

    FILE *ajuste = fopen(nombre_archivo, "r");

    if (ajuste == NULL) {
        printf("error, el archivo no se pudo abrir\n");
        return 1;
    }

    //crear la tabla
    DatosCfg datos[] = {

        //general
        {"oleadas_max", &config->oleadas_max},

        //vida
        {"vida_casa", &config->vida_casa},
        {"vida_zombie", &config->vida_zombie},
        {"vida_Flag_zombie", &config->vida_Flag_zombie},

        {"vida_girasol", &config->vida_girasol},
        {"vida_lanzaguisantes", &config->vida_lanzaguisantes},
        {"vida_nuez", &config->vida_nuez},

        //econ
        {"soles_iniciales", &config->soles_iniciales},

        {"costo_girasol", &config->costo_girasol},
        {"costo_lanzaguisantes", &config->costo_lanzaguisantes},
        {"costo_nuez", &config->costo_nuez},
        {"costo_pimenton", &config->costo_pimenton},

        {"soles_por_girasol", &config->soles_por_girasol},
        {"soles_por_generacion", &config->soles_por_generacion},

        //frecuencia y velocidad
        {"frecuencia_soles_base", &config->frecuencia_soles_base},
        {"frecuencia_soles_girasol", &config->frecuencia_soles_girasol},
        {"frecuencia_disparo", &config->frecuencia_disparo},
        {"frecuencia_generacion_zombie", &config->frecuencia_generacion_zombie},


        {"velocidad_zombie", &config->velocidad_zombie},
        {"velocidad_flag_zombie", &config->velocidad_flag_zombie},

        //tiempos
        {"tiempo_preparacion", &config->tiempo_preparacion},
        {"tiempo_descanso", &config->tiempo_descanso}
    };
    int cantidad = sizeof(datos) / sizeof(datos[0]);
    int leidos[30] = {0};

    char linea[100];
    char nombre[50];
    int valor;

    while (fgets(linea, 100, ajuste) != NULL) {
        //se ignoran los #, y lineas vacias
        char *inicio = linea + strspn(linea, " \t\r\n");

        if (*inicio == '#' || *inicio == '\0') {
        continue;
        }
        //se separa el nombre y el valor
        if (sscanf(inicio, " %49[^=] = %d", nombre, &valor) != 2) {
            printf("error en la escritura de Config.txt\n");
            fclose(ajuste);
            return 1;
        }
        nombre[strcspn(nombre, " \t\r\n")] = '\0';

        //se busca el nomnre en la tabla
int encontrado = 0;

for (int i = 0; i < cantidad; i++) {
    if (strcmp(nombre, datos[i].nombre) == 0) {
        // comprobar si el parámetro ya fue leído
        if (leidos[i] == 1) {
            printf("error, repeticion de parametro %s\n", nombre);
            fclose(ajuste);
            return 1;
        }

        //validar valores que deben ser mayores que cero
        if (strcmp(nombre, "oleadas_max") == 0 ||
            strcmp(nombre, "vida_casa") == 0 ||
            strcmp(nombre, "vida_zombie") == 0 ||
            strcmp(nombre, "vida_Flag_zombie") == 0 ||
            strcmp(nombre, "vida_girasol") == 0 ||
            strcmp(nombre, "vida_lanzaguisantes") == 0 ||
            strcmp(nombre, "vida_nuez") == 0 ||
            strcmp(nombre, "frecuencia_soles_base") == 0 ||
            strcmp(nombre, "frecuencia_soles_girasol") == 0 ||
            strcmp(nombre, "frecuencia_disparo") == 0 ||
            strcmp(nombre, "frecuencia_generacion_zombie") == 0 ||
            strcmp(nombre, "velocidad_zombie") == 0 ||
            strcmp(nombre, "velocidad_flag_zombie") == 0) {

            if (valor <= 0) {
                printf("error, %s debe ser mayor que cero\n", nombre);
                fclose(ajuste);
                return 1;
            }
        }
        else if (valor < 0) {
            printf("error, %s no puede ser negativo\n", nombre);
            fclose(ajuste);
            return 1;
        }

        // Guardar el valor
        *datos[i].valor = valor;

        leidos[i] = 1;
        encontrado = 1;

        break;
    }
}
if (encontrado == 0) {
    printf("Configuración desconocida: %s\n", nombre);
    fclose(ajuste);
    return 1;
}
}
    if (ferror(ajuste)) {
        printf("Error al leer Config.txt\n");
        fclose(ajuste);
        return 1;
    }
    for (int i = 0; i < cantidad; i++) {

        if (leidos[i] == 0) {
            printf("Falta el parámetro: %s\n", datos[i].nombre);
            fclose(ajuste);
            return 1;
        }
    }
    fclose(ajuste);

    return 0;
}
