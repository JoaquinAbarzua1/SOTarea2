#include <iostream>

#include <unistd.h> // para usar usleep()
#include <stdlib.h> // para usar rand()
using namespace std;
#include "jugadores.hpp"

// metodo para calcular adyacencia de casilla destino del puma (basicamente saber la posicion de aterrizaje del puma despues de atacar a un perro)
void calcular_salto(Coordenadas origen, Coordenadas destino, Coordenadas& adyacente) {
    // Calcular la diferencia entre las coordenadas de origen y destino
    int diff_y = origen.y - destino.y;
    int diff_x = origen.x - destino.x;

    // Calcular la posición de la casilla adyacente en la misma dirección
    adyacente.y = destino.y - diff_y;
    adyacente.x = destino.x - diff_x;
}



bool mover_puma(Tablero& tablero, int id_puma, unsigned int& semilla) {
    int origen_y = tablero.pos_puma.y;
    int origen_x = tablero.pos_puma.x;


    // antes de mover este dog, se requiere saber las casillas de adyacencias disponibles respecto a la casilla de posicion origen
    const vector<Coordenadas> vecinos = obtener_vecinos(origen_y, origen_x); // retorna las casillas adyacentes
    
    // tiro el rand para elegir un casilla X pero de las adyacentes
    int tamaño = vecinos.size();
    int indice = rand_r(&semilla) % tamaño; // la semilla del puma 

    pthread_mutex_lock(&tablero.lock_tablero); // luego de obtener el indice, se bloque el tablero o sea le ponemo candadito

    // Guardamos las coordenadas de destino
    Coordenadas destino = vecinos[indice];
    vector<Coordenadas> verificar;


    // Revisar que no esté ocupada
    if (tablero.matriz[destino.y][destino.x] == VACIO) {
        
        tablero.matriz[origen_y][origen_x] = VACIO; // Limpiar casilla actual
        tablero.matriz[destino.y][destino.x] = PUMA; // Ocupar casilla nueva

        tablero.matriz[destino.y][destino.x] = PUMA; // Ocupar casilla nueva

        tablero.pos_puma = destino; // Actualizar struct Coordenadas del cat poruqe pos_puma es un struct Coordenadas

        pthread_mutex_unlock(&tablero.lock_tablero);
        return true; // True, se movió al puma, se le avisa al juez
    }
    else if(tablero.matriz[destino.y][destino.x] == PERRO) // ahora si la casilla destino tiene un perro, entonces el puma puede atacar y como ta agresivo decide hacerlo cada vez que se le presente la oportunidad
    {
        // -REVISAR si la/s casilla/s adyacente a tablero.matriz[destino.y][destino.x] == PERRO tambien esta vacia para poder atacar
        verificar = obtener_vecinos(destino.y, destino.x); // obtengo la primera casilla adyacente a la casilla destino (donde esta el perro)

        if (verificar == VACIA){ // hay una sola casilla adyacente valida para salto, pero se debe encontrar, el como se hace lo vemo despue
        // lo importante es que como sabemos que esta vacia, se puede atacar al perro y saltar a la casilla adyacente
            calcular_salto(tablero.pos_puma, destino, verificar[0]); // calculo la casilla adyacente a la casilla destino (donde esta el perro) para saltar a ella
            tablero.matriz[origen_y][origen_x] = VACIO; // Limpiar casilla actual
            tablero.matriz[destino.y][destino.x] = VACIO; // Limpiar casilla del perro atacado

            // BUSCAMO EL PERRO Q SE PITIO EL PUMA 
            for (int i=0; i<12; i++) 
        {
            if(tablero.pos_perro[i].y == destino.y && tablero.pos_perro[i].x == destino.x){ // si la posicion del perro coincide con la posicion destino del puma, entonces ese perro es el que va a ser atacado
            
               tablero.perro_vivo[i] = false; // marco al perro como muerto
               tablero.perros_capturados++; // incremento el contador de veneco capturado pq el puma es picao a choro y se pitio a uno
               break; // puma hace lo suyo y se va
               }
            }

            tablero.matriz[verificar[0].y][verificar[0].x] = PUMA; // Ocupar casilla nueva (la casilla adyacente a la casilla destino donde estaba el perro)
            tablero.pos_puma = verificar[0]; // Actualizar la posicion del puma joder
            pthread_mutex_unlock(&tablero.lock_tablero);
            return true; // True, se movió al puma, se le avisa al juez
        }else{
            // no hay casilla adyacente vacia para saltar, entonces el puma no puede atacar y se queda en su lugar
            pthread_mutex_unlock(&tablero.lock_tablero);
            return false; // False, no se movió al puma, se le avisa al juez
    }
}}

void* rutina_puma(void* arg){
    Jugador* jugador_puma = (Jugador*)arg;

    while(true) // true == estado_partida que probablemente sea un parametro de mesa.cpp
    {
     int rango = jugador_puma->delay_max - jugador_puma->delay_min + 1;
     int retardo = jugador_puma->delay_min + (rand_r((unsigned int*)&jugador_puma->seed) % rango);
     usleep(retardo*1000);
        // rand retardo aleatorio entre 50ms y 300ms (o delays especificados en config.txt)
    mover_puma(*jugador_puma->tablero, jugador_puma->id_pieza, (unsigned int&)jugador_puma->seed);
    }
    return NULL;
}