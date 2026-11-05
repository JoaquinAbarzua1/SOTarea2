#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "parseo.hpp"
#include "tablero.hpp"
#include <vector>
#include <semaphore.h>
#include "mesa.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        cout << "ERROR: Número incorrecto de argumentos\n";
        cout << "Formato esperado: ./torneo N M ó ./torneo N M config.txt\n";
        return 1;
    }

    int N, M;

    // Validar que los parámetros sean enteros
    try {
        N = stoi(argv[1]);
        M = stoi(argv[2]);
    } catch (...) {
        cout << "ERROR: N y/o M no válidos (deben ser enteros)\n";
        return 1;
    }

    // Validar que N sea par y positivo
    if (N <= 0 || N % 2 != 0) {
        cout << "ERROR: N debe ser un número par y positivo\n";
        return 1;
    }

    // Validar que M sea positivo
    if (M <= 0) {
        cout << "ERROR: M debe ser un número positivo\n";
        return 1;
    }

    // Inicializar struct ConfigTorneo para guardar parámetros iniciales que contenga el archivo de configuración
    ConfigTorneo config;

    // Iniciar con valores no válidos para detectar si no se especifica en el archivo de configuración (porque incluso en el archivo pueden entregar mal los valores, así que igual hay que revisar si son validos o no)
    config.seed = -1; 
    config.delay_min = -1;
    config.delay_max = -1;

    config.p_inicial_puma = false;
    config.num_perros = 0;

    // Si se recibe un archivo de configuración, se parsea
    if (argc == 4) {
        if (!parseo(argv[3], config)) {
            cout << "ERROR: No se pudo parsear archivo\n";
            return 1;
        }
    }
    // Validar que los parámetros de configuración sean válidos
    if (!config_valida(config)) {
        cout << "ERROR: Configuración inválida\n";
        return 1;
    }

    int total_partidas = N / 2; // 2 jugadores por mesa, entonces necesito N/2 para atenderlos a todos

    sem_t max_mesas;
    sem_init(&max_mesas, 0, M); // Se inicia semafoto en M (mesas disponibles)

    vector <pthread_t> hebras_mesas(M); 
    vector <Mesa> datos_mesas(M);

    // Al inicio las M mesas están libres y ninguna ha sido usada todavía
    for (int k = 0; k < M; k++) {
        datos_mesas[k].mesa_libre = true;
        datos_mesas[k].usada_antes = false;
    }

    for (int i = 0; i < total_partidas; i++) {
        sem_wait(&max_mesas); // limite de M mesas concurrentes

        // Buscar cuál de las M mesas está libre
        int k = 0;
        for (int j = 0; j < M; j++) {
            if (datos_mesas[j].mesa_libre) {
                k = j;
                break;
            }
        }

        // Si ya habían jugado antes en esta mesa k, limpiamos el hilo anterior
        if (datos_mesas[k].usada_antes) {
            pthread_join(hebras_mesas[k], NULL);
        }

        datos_mesas[k].id_partida = i + 1;
        datos_mesas[k].config = config;
        datos_mesas[k].max_mesas = &max_mesas;
        datos_mesas[k].mesa_libre = false;
        datos_mesas[k].usada_antes = true;

        pthread_create(&hebras_mesas[k], NULL, rutina_mesa, (void*)&datos_mesas[k]);
    }

    // Esperar a las últimas mesas que queden jugando al final
    for (int k = 0; k < M; k++) {
        if (datos_mesas[k].usada_antes) {
            pthread_join(hebras_mesas[k], NULL);
        }
    }

    sem_destroy(&max_mesas);
    
    return 0;
}