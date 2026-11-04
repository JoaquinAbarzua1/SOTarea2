#include <iostream>
#include <unistd.h> // para usar usleep()
#include <stdlib.h> // para usar rand()
using namespace std;
#include "jugadores.hpp"
#include "mesa.hpp"


bool mover_puma(Tablero& tablero,int id_pieza, unsigned int& semilla) {
    (void) id_pieza;

    int origen_y = tablero.pos_puma.y;
    int origen_x = tablero.pos_puma.x;

    // antes de mover este cat, se requiere saber las casillas de adyacencias disponibles respecto a la casilla de posicion origen
    const vector<Coordenadas> vecinos = obtener_vecinos(origen_y, origen_x); // retorna las casillas adyacentes
    
    // tiro el rand para elegir un casilla X pero de las adyacentes
    int tamaño = vecinos.size();
    int indice = rand_r(&semilla) % tamaño; // la semilla del puma 

    // Guardamos las coordenadas de destino que el puma intenta jugar
    Coordenadas destino = vecinos[indice];

    pthread_mutex_lock(&tablero.lock_tablero); // luego de obtener el indice, se bloque el tablero o sea le ponemo candadito

    // El juez revisa y valida si el puma puede moverse o capturar en la casilla destino
    bool se_movio = validar_movimiento_puma(tablero, destino);

    pthread_mutex_unlock(&tablero.lock_tablero);
    return se_movio;
}


void* rutina_puma(void* arg){
    Jugador* jugador_puma = (Jugador*)arg;

    while (jugador_puma->tablero->partida_activa && !jugador_puma->tablero->puma_capturado)
    {
     int rango = jugador_puma->delay_max - jugador_puma->delay_min + 1;
     int retardo = jugador_puma->delay_min + (rand_r((unsigned int*)&jugador_puma->seed) % rango);
     usleep(retardo*1000);
        // rand retardo aleatorio entre 50ms y 300ms (o delays especificados en config.txt)
    mover_puma(*jugador_puma->tablero, jugador_puma->id_pieza, (unsigned int&)jugador_puma->seed);
    }
    return NULL;
}