#include <iostream>

#include <unistd.h> // para usar usleep()
#include <stdlib.h> // para usar rand()
using namespace std;
#include "jugadores.hpp"
#include "mesa.hpp"


bool mover_perro(Tablero& tablero, int id_perro, unsigned int& semilla) { // REVISAR id_perro YA QUE NO ESTA DEFINIDO
    int origen_y = tablero.pos_perro[id_perro].y;
    int origen_x = tablero.pos_perro[id_perro].x;


    // antes de mover este dog, se requiere saber las casillas de adyacencias disponibles respecto a la casilla de posicion origen
    const vector<Coordenadas> vecinos = obtener_vecinos(origen_y, origen_x); // retorna las casillas adyacentes
    
    // ahroa tiro el rand para elegir un casilla X pero de las adyacentes
    int tamaño = vecinos.size();
    int indice = rand_r(&semilla) % tamaño; // la semilla del perro 

    // Guardamos las coordenadas de destino que el perro intenta jugar
    Coordenadas destino = vecinos[indice];

    pthread_mutex_lock(&tablero.lock_tablero); // luego de obtener el indice, se bloque el tablero o sea le ponemo candadito

    // Si el perro sigue vivo y el juez le preguntó si sigue activo, responde cambiando el bool a false
    if (tablero.perro_vivo[id_perro] && tablero.verificar_conexion[id_perro]) {
        tablero.verificar_conexion[id_perro] = false;
    }

    if (!tablero.partida_activa || !tablero.perro_vivo[id_perro] || tablero.peticion_pendiente[id_perro]) {
        pthread_mutex_unlock(&tablero.lock_tablero);
        return false;
    }

    tablero.peticion_pendiente[id_perro] = true; // Marca que este perro ya tiene petición pendiente en la cola
    
// doggis se comunica con dios y le pide permiso para moverse
    Peticion peticion_nueva;
    peticion_nueva.tipo = PERRO;
    peticion_nueva.id_pieza = id_perro;
    peticion_nueva.origen = {origen_y, origen_x};
    peticion_nueva.destino = destino;

    tablero.cola_peticiones.push_back(peticion_nueva); // se mete esta nueva peticion en el vector de peticiones

    pthread_mutex_unlock(&tablero.lock_tablero);
    return true;

}

void* rutina_perro(void* arg){
    Jugador* jugador_perro = (Jugador*)arg;

    while(jugador_perro->tablero->partida_activa && jugador_perro->tablero->perro_vivo[jugador_perro->id_pieza])
    {
     int rango = jugador_perro->delay_max - jugador_perro->delay_min + 1;
     int retardo = jugador_perro->delay_min + (rand_r((unsigned int*)&jugador_perro->seed) % rango);
     usleep(retardo*1000);
        // rand retardo aleatorio entre 50ms y 300ms (o delays especificados en config.txt)
    mover_perro(*jugador_perro->tablero, jugador_perro->id_pieza, (unsigned int&)jugador_perro->seed);
    }
    return NULL;
}