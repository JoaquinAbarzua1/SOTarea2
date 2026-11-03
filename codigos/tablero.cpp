#include "tablero.hpp"
#include <iostream>
using namespace std;

// Tabla de adyacencias del Komikan (48 aristas en total)
const vector<Coordenadas> ADYACENCIAS[7][5] = {
    // Fila 0: Trono (solo (0,2) es nodo válido)
    {
        {}, // (0,0) inválido
        {}, // (0,1) inválido
        { {1, 1}, {1, 3}, {2, 2} }, // (0,2) T: conecta con (1,1), (1,3) y (2,2)
        {}, // (0,3) inválido
        {}  // (0,4) inválido
    }, 

    // Fila 1: Trono (solo (1,1) y (1,3) son nodos válidos; (1,2) es solo un cruce)
    {
        {}, // (1,0) inválido
        { {0, 2}, {1, 3}, {2, 0}, {2, 1} }, // (1,1): conecta con T, (1,3), (2,0) diagonal, (2,1) vertical
        {}, // (1,2) cruce (NO es nodo)
        { {0, 2}, {1, 1}, {2, 3}, {2, 4} }, // (1,3): conecta con T, (1,1), (2,3) vertical, (2,4) diagonal
        {}  // (1,4) inválido
    },

    // Fila 2: Cuadrícula superior (conexiones ortogonales + diagonales/verticales del trono)
    {
        { {1, 1}, {2, 1}, {3, 0} },                 // (2,0)
        { {1, 1}, {2, 0}, {2, 2}, {3, 1} },         // (2,1)
        { {0, 2}, {2, 1}, {2, 3}, {3, 2} },         // (2,2): conecta con T(0,2) vertical
        { {1, 3}, {2, 2}, {2, 4}, {3, 3} },         // (2,3)
        { {1, 3}, {2, 3}, {3, 4} }                  // (2,4)
    },

    // Fila 3: Cuadrícula (solo conexiones ortogonales)
    {
        { {2, 0}, {3, 1}, {4, 0} },                 // (3,0)
        { {2, 1}, {3, 0}, {3, 2}, {4, 1} },         // (3,1)
        { {2, 2}, {3, 1}, {3, 3}, {4, 2} },         // (3,2)
        { {2, 3}, {3, 2}, {3, 4}, {4, 3} },         // (3,3)
        { {2, 4}, {3, 3}, {4, 4} }                  // (3,4)
    },

    // Fila 4: Cuadrícula (solo conexiones ortogonales)
    {
        { {3, 0}, {4, 1}, {5, 0} },                 // (4,0)
        { {3, 1}, {4, 0}, {4, 2}, {5, 1} },         // (4,1)
        { {3, 2}, {4, 1}, {4, 3}, {5, 2} },         // (4,2)
        { {3, 3}, {4, 2}, {4, 4}, {5, 3} },         // (4,3)
        { {3, 4}, {4, 3}, {5, 4} }                  // (4,4)
    },

    // Fila 5: Cuadrícula (solo conexiones ortogonales)
    {
        { {4, 0}, {5, 1}, {6, 0} },                 // (5,0)
        { {4, 1}, {5, 0}, {5, 2}, {6, 1} },         // (5,1)
        { {4, 2}, {5, 1}, {5, 3}, {6, 2} },         // (5,2)
        { {4, 3}, {5, 2}, {5, 4}, {6, 3} },         // (5,3)
        { {4, 4}, {5, 3}, {6, 4} }                  // (5,4)
    },

    // Fila 6: Base del tablero (solo conexiones ortogonales)
    {
        { {5, 0}, {6, 1} },                         // (6,0)
        { {5, 1}, {6, 0}, {6, 2} },                 // (6,1)
        { {5, 2}, {6, 1}, {6, 3} },                 // (6,2)
        { {5, 3}, {6, 2}, {6, 4} },                 // (6,3)
        { {5, 4}, {6, 3} }                          // (6,4)
    }
};

const vector<Coordenadas>& obtener_vecinos(int y, int x) {
    return ADYACENCIAS[y][x];
}

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

void imprimir_tablero(const Tablero& tablero) {
    cout << " Estado inicial de Tablero/s:" << endl;
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 5; ++j) {
            int casilla = tablero.matriz[i][j];
            if (casilla == INVALIDO) {
                cout << "   "; 
            } else if (casilla == VACIO) {
                cout << " . "; // Nodo libre
            } else if (casilla == PUMA) {
                cout << " P "; // Puma
            } else if (casilla == PERRO) {
                cout << " D "; // Perro (Doggo)
            }
        }
        cout << "\n";
    }
}
