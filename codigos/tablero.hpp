#ifndef TABLERO_HPP
#define TABLERO_HPP

#include <pthread.h>
#include <vector>
#include <fstream> 
#include "parseo.hpp" // para usar la funcion coordenada_valida y la estructura ConfigTorneo (para usar la seed, coordenadas, delay, etc)
using namespace std;

#define INVALIDO -1
#define VACIO 0
#define PUMA 1
#define PERRO 2

struct Tablero {
    int matriz[7][5];

    // Posiciones y estado de las fichas en el tablero
    Coordenadas pos_puma; // posición del puma en el tablero
    Coordenadas pos_perro[12]; // posiciones de los perros en el tablero
    bool perro_vivo[12]; // para saber si el perro está vivo o no (si es false, el perro fue comido por el puma)
    int perros_capturados; // cantidad de perros capturados por el puma, para saber si el puma ganó (si captura 6 perros, gana)
    bool puma_capturado; 

    // Sincronización
    pthread_mutex_t lock_tablero; // Tablero completo es sección crítica (con 12 perros moviéndose al mismo tiempo es dificil asegurarse con un semáforo)

    bool partida_activa; // Juez controla estado de la partida
    int id_partida; // Identificador de la partida para el registro de eventos
    ofstream archivo_log; // Archivo de log individual de la partida

};

void iniciar_tablero(Tablero& tablero, const ConfigTorneo& config); // Inicializa el tablero con las posiciones de las fichas según la configuración del torneo
void imprimir_tablero(const Tablero& tablero); // Para revisar que el tablero se inicializó correctamente con sus coordenadas
void guardar_tablero_log(Tablero& tablero); // Guarda el estado actual (imprime matriz con las posiciones) del tablero en el archivo .log de la partida
const vector<Coordenadas>& obtener_vecinos(int y, int x); // Retorna las casillas adyacentes a (y, x)

#endif 