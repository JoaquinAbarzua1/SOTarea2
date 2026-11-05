#ifndef MESA_HPP
#define MESA_HPP

#include <semaphore.h> 
#include "tablero.hpp"
#include "parseo.hpp"

extern bool torneo_abortado;

struct Mesa
{
    int id_partida;
    ConfigTorneo config;
    sem_t* max_mesas;
};

bool puede_saltar(const Tablero& tablero, int origen_y, int origen_x, Coordenadas destino, Coordenadas& aterrizaje);
bool validar_movimiento_perro(Tablero& tablero, int id_perro, Coordenadas destino);
bool validar_movimiento_puma(Tablero& tablero, Coordenadas destino);
bool condiciones_victoria(Tablero& tablero, int id_partida);
bool verificar_jugadores_activos(Tablero& tablero, int id_partida);
void* rutina_mesa(void* arg);

#endif