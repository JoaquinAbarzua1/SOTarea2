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