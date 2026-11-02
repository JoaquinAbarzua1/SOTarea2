#ifndef JUGADORES_HPP // para evitar la doble inclsuion
#define JUGADORES_HPP

#include <pthread.h> // cabecera para la programacion con hilos (pthread_create(), pthread_join() y mas cosas)
#include "tablero.hpp" // se incluye la clase tablero

// para pasar múltiples variables al hilo sin usar variables globales (mala practica)
struct Jugador{
    Tablero* tablero; // puntero a la clase Tablero, como solo puedo pasarle un unico argumento (cosas de los hilos), mejor le paso la clase 
    int id_pieza;
    int tipo;

    int id_partida; // id de la mesa/partida
    int seed;
    int delay_min;
    int delay_max; // los retardos (50 para min y 300 para max en ms)
}; // tonces, el hilo se conforma de un id, sus retardos y lo

// Declarar rutinas, logicas de esto, en las clases puma.cpp y perro.cpp
void* rutina_perro(void* arg); // la rutina del hilo solo puede recibir un unico parametro (el puntero generico void)
void* rutina_puma(void* arg);


#endif