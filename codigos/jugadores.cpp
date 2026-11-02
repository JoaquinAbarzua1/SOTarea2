#include <iostream>

#include <unistd.h> // para usar usleep()
#include <stdlib.h> // para usar rand()
using namespace std;
#include "jugadores.hpp"

void iniciar_jugadores(Tablero& tablero, int delay_min, int delay_max){
    pthread_t hebra_puma;
    pthread_t hebras_perro[12];

    // Se define el static para que las variables no exploten al salir de la funcion y asi las hebras no lean  basura
    
}

// LOGICA DEL DOG
void* rutina_perro(void* arg){

    // entonces, primero lo primero -> desempaquetar el struct Jugador que contiene los datos del jugador
    Jugador* jugador = (Jugador*) arg; // casteo el jugador, que contiene el tablero, id de la pieza, tipo, id de la partida y los retardos

    // logica del movimiento perruno
    while(true) // en realidad este while, debiese ser un while que se rompa cuando el juego termine, pero por ahora lo dejo asi
    {
        // mientras se pueda mover la ficha, se sigue moviendo (usar funcion mover_ficha() para validar si se puede mover o no)
        // si el destino esta ocupado, se busca otro destino aleatorio hasta que se pueda mover la ficha
        


        // RETARDO COSAS  (aunque podria usar una funcion chica para evaluar estos retardos y usarlo en la logica de puma y perro)
        if (jugador->delay_min == NULL || jugador->delay_max == NULL) {
            cout << "Error: delay_min o delay_max no están definidos para el jugador con id_pieza " << jugador->id_pieza << endl;
        } else {
            // Si se especifican retardos, se hace un sleep aleatorio entre delay_min y delay_max
            int delay = jugador->delay_min + rand() % (jugador->delay_max - jugador->delay_min + 1);
            usleep(delay * 1000); 
        }
    }


};