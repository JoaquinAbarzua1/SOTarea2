#include "tablero.h"
using namespace std;

void iniciar_tablero(Tablero& tablero) {
    // Inicializar la matriz del tablero con VACIO
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 5; ++j) {
            tablero.matriz[i][j] = VACIO;
        }
    }

    // Iniciar el mutex del tablero
    pthread_mutex_init(&tablero.lock_tablero, NULL);

    // Bloquear casillas del trono para que quede triangular
    tablero.matriz[0][0] = INVALIDO;
    tablero.matriz[0][1] = INVALIDO;
    tablero.matriz[0][3] = INVALIDO;
    tablero.matriz[0][4] = INVALIDO;
    tablero.matriz[1][0] = INVALIDO;
    tablero.matriz[1][4] = INVALIDO;
}