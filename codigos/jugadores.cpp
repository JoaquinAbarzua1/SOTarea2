#include <iostream>

#include <unistd.h> // para usar usleep()
#include <stdlib.h> // para usar rand()
using namespace std;
#include "jugadores.hpp"

void iniciar_jugadores(Tablero& tablero, const ConfigTorneo& config, int id_partida, pthread_t& hebra_puma, pthread_t hebras_perro[12], Jugador& datos_puma, Jugador datos_perros[12]){
    // Datos del puma
    datos_puma.tablero = &tablero;
    datos_puma.id_pieza = 12;
    datos_puma.tipo = PUMA;
    datos_puma.id_partida = id_partida;
    datos_puma.delay_min = config.delay_min;
    datos_puma.delay_max = config.delay_max;

    datos_puma.seed = config.seed + id_partida;

    pthread_create(&hebra_puma, NULL, rutina_puma, (void*)&datos_puma);
    // Datos perros
    for (int i = 0; i < 12; i++) {
        datos_perros[i].tablero = &tablero;
        datos_perros[i].id_pieza = i;
        datos_perros[i].tipo = PERRO;
        datos_perros[i].id_partida = id_partida;
        datos_perros[i].delay_min = config.delay_min;
        datos_perros[i].delay_max = config.delay_max;

        datos_perros[i].seed = config.seed + id_partida + i;

        pthread_create(&hebras_perro[i], NULL, rutina_perro, (void*)&datos_perros[i]);
    }
    
}