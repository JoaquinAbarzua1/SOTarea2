#ifndef PARSEO_H
#define PARSEO_H

using namespace std;

// Estructura con parámetros de la configuración del torneo
struct ConfigTorneo {
    int seed;
    int delay_min;
    int delay_max;
};

bool parseo(const string& nombreArchivo, ConfigTorneo& config);

#endif