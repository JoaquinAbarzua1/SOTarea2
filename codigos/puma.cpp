#include <iostream>

#include <unistd.h> // para usar usleep()
#include <stdlib.h> // para usar rand()
using namespace std;
#include "jugadores.hpp"


bool mover_puma(Tablero& tablero, int id_puma, unsigned int& semilla) {
    int origen_y = tablero.pos_puma[0].y;
    int origen_x = tablero.pos_puma[0].x;


    // antes de mover este dog, se requiere saber las casillas de adyacencias disponibles respecto a la casilla de posicion origen
    const vector<Coordenadas> vecinos = obtener_vecinos(origen_y, origen_x); // retorna las casillas adyacentes
    
    // ahroa tiro el rand para elegir un casilla X pero de las adyacentes
    int tamaño = vecinos.size();
    int indice = rand_r(&semilla) % tamaño; // la semilla del puma 

    pthread_mutex_lock(&tablero.lock_tablero); // luego de obtener el indice, se bloque el tablero o sea le ponemo candadito

    // Guardamos las coordenadas de destino
    Coordenadas destino = vecinos[indice];

    // Revisar que no esté ocupada
    if (tablero.matriz[destino.y][destino.x] == VACIO) {
        
        tablero.matriz[origen_y][origen_x] = VACIO; // Limpiar casilla actual
        tablero.matriz[destino.y][destino.x] = PUMA; // Ocupar casilla nueva

        tablero.matriz[destino.y][destino.x] = PUMA; // Ocupar casilla nueva

        tablero.matriz[destino.y][destino.x] = PUMA; // Ocupar casilla nueva

        tablero.pos_puma[id_puma] = destino; // Actualizar struct Coordenadas del Doggo

        pthread_mutex_unlock(&tablero.lock_tablero);
        return true;
    }
    else if(tablero.matriz[destino.y][destino.x] == PERRO){
        // -REVISAR si la/s casilla/s adyacente a tablero.matriz[destino.y][destino.x] == PERRO tambien esta vacia para poder atacar
        // PD: Hay que calcular la direccion del salto para saber si la casilla adyacente está en una direccion permitida por las reglas del juego, que vendria a ser una "linea recta"
        // Una propuesta de soluciona para el calculo de direccion del salto es realizar la diferencia del nodo donde se encuentra el puma y el nodo donde se encuentra el perro, al nodo donde se encuentra el perro se le suma la diferencia para dar con la posicion de la casilla a la cual el puma llegara de un salto.
        // se me ocurren los ifs correspondientes...
        // if(vacia == true){atacar()} 
        // else {entonces simplemente no hay ataque y continua el rumbo}
        // - OJO: SI SE LOGRO ENTRAR AL IF, ENTONCES SACAR AL PERRO DE LA CASILLA (MATARLO), por ende necesito conocer el id_perro y aumentar contador de perros capturados.
        
    }
    pthread_mutex_unlock(&tablero.lock_tablero); // La casilla no estaba vacía, no se mueve (para evitar dejar tablero bloqueado y seguir buscando)
    return false; // False, no se movió al puma, no se le avisa al juez  
}

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