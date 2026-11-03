#include <iostream>
#include <unistd.h> // para usar usleep()
#include <stdlib.h> // para usar rand()
using namespace std;
#include "jugadores.hpp"


bool mover_puma(Tablero& tablero,int id_pieza, unsigned int& semilla) {
    (void) id_pieza;

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

    // Revisar que no esté ocupada
    if (tablero.matriz[destino.y][destino.x] == VACIO) {
        
        tablero.matriz[origen_y][origen_x] = VACIO; // Limpiar casilla actual
        tablero.matriz[destino.y][destino.x] = PUMA; // Ocupar casilla nueva
        tablero.pos_puma = destino; // Actualizar struct Coordenadas del cat poruqe pos_puma es un struct Coordenadas

        pthread_mutex_unlock(&tablero.lock_tablero);
        return true; // True, se movió al puma, se le avisa al juez
    }
    
    else if(tablero.matriz[destino.y][destino.x] == PERRO) // ahora si la casilla destino tiene un perro, entonces el puma puede atacar y como ta agresivo decide hacerlo cada vez que se le presente la oportunidad
    {
        Coordenadas aterrizaje;
        // Caso exclusivo dado que en el trono hay una coordenada q no vale (el 1,2)
        if (origen_x == 2 && destino.x == 2) {
            if (origen_y == 0 && destino.y == 2) aterrizaje = {3, 2};
            else if (origen_y == 3 && destino.y == 2) aterrizaje = {0, 2};
            else aterrizaje = {destino.y + (destino.y - origen_y), destino.x};

        } else {
            int diff_y = destino.y - origen_y;
            int diff_x = destino.x - origen_x;
            aterrizaje.y = destino.y + diff_y;
            aterrizaje.x = destino.x + diff_x;
        }
        
        const vector<Coordenadas> vecinos_perro = obtener_vecinos(destino.y, destino.x);
        bool es_vecino_valido = false;
        for (int i = 0; i < vecinos_perro.size(); i++) {
            if (vecinos_perro[i].y == aterrizaje.y && vecinos_perro[i].x == aterrizaje.x) {
                es_vecino_valido = true;
                break;
            }
        }

        if (es_vecino_valido && tablero.matriz[aterrizaje.y][aterrizaje.x] == VACIO) {
            tablero.matriz[origen_y][origen_x] = VACIO;
            tablero.matriz[destino.y][destino.x] = VACIO;
            tablero.matriz[aterrizaje.y][aterrizaje.x] = PUMA;
            tablero.pos_puma = aterrizaje;

                    // BUSCAMO EL PERRO Q SE PITIO EL PUMA 
            for (int i=0; i<12; i++) 
            {
            
                if(tablero.pos_perro[i].y == destino.y && tablero.pos_perro[i].x == destino.x) { // si la posicion del perro coincide con la posicion destino del puma, entonces ese perro es el que va a ser atacado
        
                    tablero.perro_vivo[i] = false; // marco al perro como muerto
                    tablero.perros_capturados++; // incremento el contador de veneco capturado pq el puma es picao a choro y se pitio a uno
                    break; // puma hace lo suyo y se va
                }
            }
            
        pthread_mutex_unlock(&tablero.lock_tablero);
        return true; // True, se movió al puma, se le avisa al juez
        }
           

    }
    // no hay casilla adyacente vacia para saltar, entonces el puma no puede atacar y se queda en su lugar
    pthread_mutex_unlock(&tablero.lock_tablero);
    return false; // False, no se movió al puma, se le avisa al juez
    };


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