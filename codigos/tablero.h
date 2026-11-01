#ifndef TABLERO_H
#define TABLERO_H

#include <pthread.h>
using namespace std;

#define INVALIDO -1
#define VACIO 0
#define PUMA 1
#define PERRO 2

struct Tablero {
    int matriz[7][5];
    pthread_mutex_t lock_tablero; // Tablero completo es sección crítica (con 12 perros moviéndose al mismo tiempo es dificil asegurarse con un semáforo)
};

void iniciar_tablero(Tablero& tablero);
bool mover_ficha(Tablero& tablero, int origen_x, int origen_y, int destino_x, int destino_y);

#endif 