#include "parseo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

bool parseo(const string& nombreArchivo, ConfigTorneo& config) {
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo de configuración\n";
        return false;
    }

    // string para guardar cada linea del archivo
    string linea;
    
    // Leer el archivo línea por línea
    while (getline(archivo, linea)) {
        // Ignorar líneas vacías
        if (linea.empty()) continue;

        stringstream ss(linea); // stringstream para separar la línea en palabras
        string parametro; // Variable para guardar cada palabra

        // Mientras haya palabras en la línea, leerlas
        while (ss >> parametro) {
            if (parametro == "SEED") {
                // Intentar guardar el siguiente valor en config.seed (si no es un número, retorna error)
                if (!(ss >> config.seed)) {
                    cout << "ERROR: Valor de SEED no válido\n";
                    return false;
                }
            }

            else if (parametro == "DELAY_MIN") {
                if (!(ss >> config.delay_min) || config.delay_min < 0) {
                    cout << "ERROR: Valor de DELAY_MIN no válido\n";
                    return false;
                }
            }

            else if (parametro == "DELAY_MAX") {
                if (!(ss >> config.delay_max) || config.delay_max < 0) {
                    cout << "ERROR: Valor de DELAY_MAX no válido\n";
                    return false;
                }
            }
        }
    }

    archivo.close();

    // Caso borde: Si no se especifica SEED, DELAY_MIN o DELAY_MAX en el archivo, se asignan valores por defecto
    if (config.seed == -1) {
        config.seed = 67; // Valor por defecto para SEED
    }
    if (config.delay_min == -1) {
        config.delay_min = 50; // Valor por defecto para DELAY_MIN
    }
    if (config.delay_max == -1) {
        config.delay_max = 300; // Valor por defecto para DELAY_MAX
    }

    // Caso borde: Si DELAY_MIN es mayor que DELAY_MAX, se intercambian los valores
    if (config.delay_min > config.delay_max) {
        cout << "ERROR SINTAXIS: DELAY_MIN es mayor que DELAY_MAX, se intercambian los valores para corregir error\n";
        swap(config.delay_min, config.delay_max);
    }

    cout << "Parámetros de configuración:\n";
    cout << "SEED: " << config.seed << "\n";
    cout << "DELAY_MIN: " << config.delay_min << "\n";
    cout << "DELAY_MAX: " << config.delay_max << "\n";
    
    return true;
}
