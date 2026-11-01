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

bool mover_ficha(Tablero& tablero, int origen_x, int origen_y, int destino_x, int destino_y) {
    // 1. Solicitar acceso exclusivo a la Sección Crítica
    pthread_mutex_lock(&tablero.lock_tablero);

    // 2. Validar que el destino esté dentro de los límites de la matriz 7x5
    if (destino_x < 0 || destino_x > 4 || destino_y < 0 || destino_y > 6) {
        pthread_mutex_unlock(&tablero.lock_tablero);
        return false;
    }

    // 3. Validar que el destino no sea una esquina fantasma del trono
    if (tablero.matriz[destino_y][destino_x] == INVALIDO) {
        pthread_mutex_unlock(&tablero.lock_tablero);
        return false;
    }

    // 4. Validar que la casilla de destino esté realmente libre
    if (tablero.matriz[destino_y][destino_x] != VACIO) {
        pthread_mutex_unlock(&tablero.lock_tablero);
        return false;
    }

    // 5. Si todo es válido, ejecutar el movimiento
    int ficha = tablero.matriz[origen_y][origen_x];
    tablero.matriz[origen_y][origen_x] = VACIO;
    tablero.matriz[destino_y][destino_x] = ficha;

    // 6. Liberar el candado para que otros hilos puedan evaluar sus movimientos
    pthread_mutex_unlock(&tablero.lock_tablero);
    
    return true;
}