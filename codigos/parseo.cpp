#include "parseo.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "tablero.hpp"
using namespace std;

bool coordenada_valida(int y, int x) {
    // Validar que las coordenadas estén dentro de los límites de la matriz 7x5
    if (y < 0 || y > 6 || x < 0 || x > 4) {
        return false;
    }

    // Validar que las coordenadas no sean una esquina prohibida del trono
    if ((y == 0 && (x != 2)) || (y == 1 && (x != 1 && x != 3))) {
        return false;
    }

    return true;
}

// Verifica si una coordenada choca con las fichas ya registradas
bool casilla_ocupada(int y, int x, const ConfigTorneo& config) {
    // Revisar colisión con el Puma (solo si ya fue leído)
    if (config.p_inicial_puma && config.puma.y == y && config.puma.x == x) {
        return true;
    }

    // Revisar colisión con los perros guardados hasta el momento
    for (int i = 0; i < config.num_perros; i++) {
        if (config.perro[i].y == y && config.perro[i].x == x) {
            return true;
        }
    }

    return false;
}

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

            else if (parametro == "PUMA") {
                int y, x;
                if (config.p_inicial_puma) { 
                    cout << "ERROR: PUMA repetido\n"; 
                    return false; 
                }

                if (!(ss >> y >> x)) { 
                    cout << "ERROR: Valores de PUMA no válidos\n"; 
                    return false; 
                }
                
                if (!coordenada_valida(y, x) || casilla_ocupada(y, x, config)) {
                    cout << "ERROR: Posición de PUMA inválida u ocupada\n"; 
                    return false;
                }
                config.puma.y = y; config.puma.x = x;
                config.p_inicial_puma = true;
            }

            else if (parametro == "PERRO") {
                int y, x; // Reviso todas las coordenadas entregadas
                while (config.num_perros < 12 && ss >> y >> x ) {
                    if (coordenada_valida(y, x) && !casilla_ocupada(y, x, config)) {
                        config.perro[config.num_perros].x = x;
                        config.perro[config.num_perros].y = y;
                        config.num_perros++;
                    } else {
                        cout << "ERROR: Coordenadas de PERRO no válidas (coordenada ocupada o fuera de los límites)\n";
                        return false;
                    }
                }
            }

            else {
                cout << "ERROR: Parámetro desconocido en archivo de configuración: " << parametro << "\n";
                return false;
            }
        }
    }

    archivo.close();
    return true;
}

bool config_valida(ConfigTorneo& config) {
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
        cout << "ERROR: DELAY_MIN es mayor que DELAY_MAX. Situación inválida\n";
        return false;
    }

    if (config.p_inicial_puma && config.num_perros == 12) {
        cout << "INFO: Se cargaron posiciones de las fichas desde la configuración.\n";
    } 

    else if (!config.p_inicial_puma && config.num_perros == 0) {
        config.puma.y = 0; 
        config.puma.x = 2;
        config.p_inicial_puma = true;

        config.perro[0] = {6, 0}; 
        config.perro[1] = {6, 1}; 
        config.perro[2] = {6, 2}; 
        config.perro[3] = {6, 3}; 
        config.perro[4] = {6, 4}; 
        config.perro[5] = {5, 0}; 
        config.perro[6] = {5, 1}; 
        config.perro[7] = {5, 2}; 
        config.perro[8] = {5, 3}; 
        config.perro[9] = {5, 4}; 
        config.perro[10] = {4, 0}; 
        config.perro[11] = {4, 4};
        config.num_perros = 12;

        cout << "No se especificaron posiciones. Se asignan las posiciones por defecto del tablero.\n";
    } 

    else {
        cout << "ERROR: Posiciones de fichas incompletas. Se requiere especificar todas las posiciones de las fichas o ninguna.\n";
        return false;
    }

    cout << "Parámetros de configuración:\n";
    cout << "SEED: " << config.seed << "\n";
    cout << "DELAY_MIN: " << config.delay_min << "\n";
    cout << "DELAY_MAX: " << config.delay_max << "\n";
    cout << "PUMA: (" << config.puma.y << ", " << config.puma.x << ")\n";
    cout << "PERROS: ";
    for (int i = 0; i < config.num_perros; i++) {
        cout << "(" << config.perro[i].y << ", " << config.perro[i].x << ")";
        if (i < config.num_perros - 1) {
            cout << ", ";
        }
    }
    cout << "\n";
    
    return true;
}
