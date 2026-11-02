#ifndef FICHA_HPP
#define FICHA_HPP

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include "jugadores.hpp" // este ya tiene el tablero.hpp incluido, asi que no hace falta incluirlo de nuevo

// LOGICA DEL DOG
void* rutina_perro(void* arg){

    // entonces, primero lo primero -> desempaquetar el struct Jugador que contiene los datos del jugador
    Jugador* jugador = (Jugador*) arg; // casteo el jugador, que contiene el tablero, id de la pieza, tipo, id de la partida y los retardos

    // logica del movimiento perruno
    while(true) // en realidad este while, debiese ser un while que se rompa cuando el juego termine, pero por ahora lo dejo asi
    {
        // mientras se pueda mover la ficha, se sigue moviendo

        if (jugador->delay_min == NULL || jugador->delay_max == NULL) {
            // Si no se especifican retardos, no se hace nada
        } else {
            // Si se especifican retardos, se hace un sleep aleatorio entre delay_min y delay_max
            int delay = jugador->delay_min + rand() % (jugador->delay_max - jugador->delay_min + 1);
            usleep(delay * 1000); // convertir a microsegundos
        }
    }
    


};







// LOGICA DEL CAT 

#endif