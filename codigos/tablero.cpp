#include "tablero.hpp"

using namespace std;

void iniciar_tablero(Tablero& tablero, const ConfigTorneo& config) {
    // Inicializar la matriz del tablero con VACIO o INVALIDO según corresponda
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 5; ++j) { 
            if(coordenada_valida(i,j)==true){
            tablero.matriz[i][j] = VACIO;
            } else { tablero.matriz[i][j] = INVALIDO; } 
        }
    }

    // Colocar PUMA en su posición inicial
    tablero.pos_puma = config.puma;
    tablero.matriz[tablero.pos_puma.y][tablero.pos_puma.x] = PUMA; // Marcar casilla con PUMA
    tablero.puma_capturado = false; 

    // Colocar a los 12 PERROS en sus posiciones iniciales
    for (int i = 0; i < 12; ++i) {
        tablero.pos_perro[i] = config.perro[i];
        tablero.matriz[config.perro[i].y][config.perro[i].x] = PERRO; // Marcar casilla con PERRO
        tablero.perro_vivo[i] = true;
    }

    tablero.perros_capturados = 0;




    // Iniciar el mutex del tablero
    pthread_mutex_init(&tablero.lock_tablero, NULL);
}

bool mover_ficha(Tablero& tablero, int origen_x, int origen_y, int destino_x, int destino_y) {
    // Solicitar acceso exclusivo a la sección crítica
    pthread_mutex_lock(&tablero.lock_tablero);

    // Validar que el destino esté dentro de los límites de la matriz 7x5
    if (destino_x < 0 || destino_x > 4 || destino_y < 0 || destino_y > 6) {
        pthread_mutex_unlock(&tablero.lock_tablero);
        return false;
    }

    // Validar que el destino no sea una esquina fantasma del trono
    if (tablero.matriz[destino_y][destino_x] == INVALIDO) {
        pthread_mutex_unlock(&tablero.lock_tablero);
        return false;
    }

    // Validar que la casilla de destino esté realmente libre
    if (tablero.matriz[destino_y][destino_x] != VACIO) {
        pthread_mutex_unlock(&tablero.lock_tablero);
        return false;
    }

    // Si todo es válido, ejecutar el movimiento
    int ficha = tablero.matriz[origen_y][origen_x];
    tablero.matriz[origen_y][origen_x] = VACIO;
    tablero.matriz[destino_y][destino_x] = ficha;

    // Liberar el candado para que otros hilos puedan evaluar sus movimientos
    pthread_mutex_unlock(&tablero.lock_tablero);
    
    return true;
}
