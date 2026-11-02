#ifndef PARSEO_HPP
#define PARSEO_HPP
#include<string>

using namespace std;

struct Coordenadas {
    int y; // fila
    int x; // columna
};

// Estructura con parámetros de la configuración del torneo
struct ConfigTorneo {
    int seed;
    int delay_min;
    int delay_max;
    Coordenadas puma;
    bool p_inicial_puma; // Si se entrega posición inicial del puma en el archivo de configuración
    Coordenadas perro[12];
    int num_perros; // Si se entregan posiciones iniciales de los perros en el archivo de configuración, cuántos son (podrían faltar)
};

bool parseo(const string& nombreArchivo, ConfigTorneo& config);
bool config_valida(ConfigTorneo& config);
bool coordenada_valida(int y, int x); // Para que otros archivos puedan usarla, se declara en el .hpp y se define en el .cpp

#endif