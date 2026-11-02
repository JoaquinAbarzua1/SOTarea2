#ifndef TABLERO_HPP
#define TABLERO_HPP

#include <pthread.h>
#include "parseo.hpp" // para usar la funcion coordenada_valida y la estructura ConfigTorneo (para usar la seed, coordenadas, delay, etc)
using namespace std;

#define INVALIDO -1
#define VACIO 0
#define PUMA 1
#define PERRO 2

struct Tablero {
    int matriz[7][5];

    // Posiciones y estado de las fichas en el tablero
    Coordenadas pos_puma; // posición del puma en el tablero
    Coordenadas pos_perro[12]; // posiciones de los perros en el tablero
    bool perro_vivo[12]; // para saber si el perro está vivo o no (si es false, el perro fue comido por el puma)
    int perros_capturados; // cantidad de perros capturados por el puma, para saber si el puma ganó (si captura 6 perros, gana)

    // Estado de la partida y aviso de movimientos para validación de Juez
    bool movimiento_realizado; // para avisar al juez que se realizó un movimiento (para que el juez valide si es correcto o no)
    bool partida_terminada; // Juez avisa que partida terminó
    int ganador; // 1. Puma, 2. Perro, 0 nadie (partida activa)

    // Sincronización
    pthread_mutex_t lock_tablero; // Tablero completo es sección crítica (con 12 perros moviéndose al mismo tiempo es dificil asegurarse con un semáforo)
    pthread_cond_t cond_juez;// Desbloquea al juez cada vez que se realiza un movimiento
};

void iniciar_tablero(Tablero& tablero);
bool mover_ficha(Tablero& tablero, int origen_x, int origen_y, int destino_x, int destino_y);

#endif 