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

    // Si el juez le preguntó al puma si sigue activo, responde cambiando el bool a false
    if (!tablero.puma_capturado && tablero.verificar_conexion[12]) {
        tablero.verificar_conexion[12] = false;
    }

    if (!tablero.partida_activa || tablero.puma_capturado || tablero.peticion_pendiente[12]) {
        pthread_mutex_unlock(&tablero.lock_tablero);
        return false;
    }   

    tablero.peticion_pendiente[12] = true; // Marca que el puma ya tiene petición pendiente en la cola

// gatito le pide permiso a dios para moverse
    Peticion peticion_nueva;
    peticion_nueva.tipo = PUMA;
    peticion_nueva.id_pieza = 12;
    peticion_nueva.origen = {origen_y, origen_x};
    peticion_nueva.destino = destino;

    tablero.cola_peticiones.push_back(peticion_nueva); // se mete esta nueva peticion en el vector de peticiones

    pthread_mutex_unlock(&tablero.lock_tablero);
    return true;
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