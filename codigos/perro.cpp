#include <iostream>

#include <unistd.h> // para usar usleep()
#include <stdlib.h> // para usar rand()
using namespace std;
#include "jugadores.hpp"


bool mover_perro(Tablero& tablero, int id_perro, unsigned int& semilla) { // REVISAR id_perro YA QUE NO ESTA DEFINIDO
    int origen_y = tablero.pos_perro[id_perro].y;
    int origen_x = tablero.pos_perro[id_perro].x;


    // antes de mover este dog, se requiere saber las casillas de adyacencias disponibles respecto a la casilla de posicion origen
    const vector<Coordenadas> vecinos = obtener_vecinos(origen_y, origen_x); // retorna las casillas adyacentes
    
    // ahroa tiro el rand para elegir un casilla X pero de las adyacentes
    int tamaño = vecinos.size();
    int indice = rand_r(&semilla) % tamaño; // la semilla del perro 

    pthread_mutex_lock(&tablero.lock_tablero); // luego de obtener el indice, se bloque el tablero o sea le ponemo candadito

    // Guardamos las coordenadas de destino
    Coordenadas destino = vecinos[indice];

    // Revisar que no esté ocupada
    if (tablero.matriz[destino.y][destino.x] == VACIO) {
        
        tablero.matriz[origen_y][origen_x] = VACIO; // Limpiar casilla actual
        tablero.matriz[destino.y][destino.x] = PERRO; // Ocupar casilla nueva
        tablero.pos_perro[id_perro] = destino; // Actualizar struct Coordenadas del Doggo
        pthread_mutex_unlock(&tablero.lock_tablero);
        return true;
    }

    pthread_mutex_unlock(&tablero.lock_tablero); // La casilla no estaba vacía, no se mueve (para evitar dejar tablero bloqueado y seguir buscando)
    return false; // False, no se movió al perro, no se le avisa al juez  
}

void* rutina_perro(void* arg){
    Jugador* jugador_perro = (Jugador*)arg;

    while(true) // true == estado_partida que probablemente sea un parametro de mesa.cpp
    {
     int rango = jugador_perro->delay_max - jugador_perro->delay_min + 1;
     int retardo = jugador_perro->delay_min + (rand_r((unsigned int*)&jugador_perro->seed) % rango);
     usleep(retardo*1000);
        // rand retardo aleatorio entre 50ms y 300ms (o delays especificados en config.txt)
    mover_perro(*jugador_perro->tablero, jugador_perro->id_pieza, (unsigned int&)jugador_perro->seed);
    }
    return NULL;
}