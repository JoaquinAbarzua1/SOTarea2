#include <iostream>
#include <unistd.h>
#include "mesa.hpp"
#include "jugadores.hpp"

using namespace std;

bool puede_saltar (const Tablero& tablero, int origen_y, int origen_x, Coordenadas destino, Coordenadas& aterrizaje) {

    // Caso exclusivo dado que en el trono hay una coordenada q no vale (el 1,2)
    if (origen_x == 2 && destino.x == 2) {
        if (origen_y == 0 && destino.y == 2) aterrizaje = {3, 2};
        else if (origen_y == 3 && destino.y == 2) aterrizaje = {0, 2};
        else aterrizaje = {destino.y + (destino.y - origen_y), destino.x};

    } else {
        int diff_y = destino.y - origen_y;
        int diff_x = destino.x - origen_x;
        aterrizaje.y = destino.y + diff_y;
        aterrizaje.x = destino.x + diff_x;
    }
    
    const vector<Coordenadas> vecinos_perro = obtener_vecinos(destino.y, destino.x);
    bool es_vecino_valido = false;
    for (int i = 0; i < (int)vecinos_perro.size(); i++) {
        if (vecinos_perro[i].y == aterrizaje.y && vecinos_perro[i].x == aterrizaje.x) {
            es_vecino_valido = true;
            break;
        }
    }

    return (es_vecino_valido && tablero.matriz[aterrizaje.y][aterrizaje.x] == VACIO);
}

bool validar_movimiento_perro(Tablero& tablero, int id_perro, Coordenadas destino) {
    if (!tablero.perro_vivo[id_perro] || !tablero.partida_activa) {
        return false;
    }

    int origen_y = tablero.pos_perro[id_perro].y;
    int origen_x = tablero.pos_perro[id_perro].x;

    // El juez revisa que la casilla destino no esté ocupada
    if (tablero.matriz[destino.y][destino.x] == VACIO) {
        
        tablero.matriz[origen_y][origen_x] = VACIO; // Limpiar casilla actual
        tablero.matriz[destino.y][destino.x] = PERRO; // Ocupar casilla nueva
        tablero.pos_perro[id_perro] = destino; // Actualizar struct Coordenadas del Doggo
        // Mostrar movimiento por consola y registrar en logs
        cout << "[Partida " << tablero.id_partida << "] Perro " << id_perro << " se mueve de [" << origen_y << ", " << origen_x << "] a [" << destino.y << ", " << destino.x << "]\n";
        tablero.archivo_log << "[Partida " << tablero.id_partida << "] Perro " << id_perro << " se mueve de [" << origen_y << ", " << origen_x << "] a [" << destino.y << ", " << destino.x << "]\n";
        guardar_tablero_log(tablero);

        return true;
    }

    return false; // False, la casilla no estaba vacía, el juez rechaza el movimiento del perro
}

bool validar_movimiento_puma(Tablero& tablero, Coordenadas destino) {
    if (!tablero.partida_activa || tablero.puma_capturado) {
        return false;
    }

    int origen_y = tablero.pos_puma.y;
    int origen_x = tablero.pos_puma.x;

    // El juez revisa que la casilla destino no esté ocupada
    if (tablero.matriz[destino.y][destino.x] == VACIO) {
        
        tablero.matriz[origen_y][origen_x] = VACIO; // Limpiar casilla actual
        tablero.matriz[destino.y][destino.x] = PUMA; // Ocupar casilla nueva
        tablero.pos_puma = destino; // Actualizar struct Coordenadas del cat poruqe pos_puma es un struct Coordenadas

        // Lo mismo (mostrar por consola y registrar en logs)
        cout << "[Partida " << tablero.id_partida << "] Puma se mueve de [" << origen_y << ", " << origen_x << "] a [" << destino.y << ", " << destino.x << "]\n";
        tablero.archivo_log << "[Partida " << tablero.id_partida << "] Puma se mueve de [" << origen_y << ", " << origen_x << "] a [" << destino.y << ", " << destino.x << "]\n";
        guardar_tablero_log(tablero);

        return true; // True, el juez valida y mueve al puma
    }
    
    else if(tablero.matriz[destino.y][destino.x] == PERRO) // ahora si la casilla destino tiene un perro, entonces el puma puede atacar y como ta agresivo decide hacerlo cada vez que se le presente la oportunidad
    {
        Coordenadas aterrizaje;

        // El juez revisa si el salto es posible (si es que en la casilla donde caiga no hay un perro o o sea válida)
        if (puede_saltar(tablero, origen_y, origen_x, destino, aterrizaje)) {
            tablero.matriz[origen_y][origen_x] = VACIO;
            tablero.matriz[destino.y][destino.x] = VACIO;
            tablero.matriz[aterrizaje.y][aterrizaje.x] = PUMA;
            tablero.pos_puma = aterrizaje;

            int perro_comido = -1; // para buscar el id del perro comido
            // EL JUEZ BUSCA EL PERRO Q SE PITIO EL PUMA 
            for (int i=0; i<12; i++) 
            {
            
                if(tablero.pos_perro[i].y == destino.y && tablero.pos_perro[i].x == destino.x && tablero.perro_vivo[i]) { // si la posicion del perro coincide con la posicion destino del puma, entonces ese perro es el que va a ser atacado
        
                    tablero.perro_vivo[i] = false; // el juez marca al perro como muerto
                    tablero.perros_capturados++; // incrementa el contador de veneco capturado pq el puma es picao a choro y se pitio a uno
                    perro_comido = i; // se guarda el perro comido
                    break; // puma hace lo suyo y se va
                }
            }
            // Mostrar que puma se come a perro por consola y registrar en logs
            cout << "[Partida " << tablero.id_partida << "] RAWR (sonido de gato furioso) Puma salta de [" << origen_y << ", " << origen_x << "] a [" << aterrizaje.y << ", " << aterrizaje.x << "] y captura a Perro " << perro_comido << " en [" << destino.y << ", " << destino.x << "] (Capturados: " << tablero.perros_capturados << "/6)\n";
            tablero.archivo_log << "[Partida " << tablero.id_partida << "] RAWR (sonido de gato furioso) Puma salta de [" << origen_y << ", " << origen_x << "] a [" << aterrizaje.y << ", " << aterrizaje.x << "] y captura a Perro " << perro_comido << " en [" << destino.y << ", " << destino.x << "] (Capturados: " << tablero.perros_capturados << "/6)\n";
            guardar_tablero_log(tablero);

            return true; // True, el juez valida la captura y mueve al puma
        }
    }

    // LLEGADO A ESTE PUNTO, EL PUMA NO SE MOVIO, ENTONCES EL JUEZ DEBE REVISAR SI TIENE ESCAPATORIA
    const vector<Coordenadas> vecinos = obtener_vecinos(origen_y, origen_x);
    bool escape = false;
    // El juez revisa las casillas adyacentes al puma
    for (int k = 0; k < (int)vecinos.size(); k++) {
        Coordenadas destino_escape = vecinos[k];
        Coordenadas aterrizaje_escape; // Para enviarle a "puede_saltar" 

        if (tablero.matriz[destino_escape.y][destino_escape.x] == VACIO || puede_saltar(tablero, origen_y, origen_x, destino_escape, aterrizaje_escape)) {
            escape = true;
            break; // Hay un movimiento disponible, puede escapar en el siguiente intento de moverse
        }
    }

    // Si no se actualizó "escape" es porque no hubo movimiento disponible, entonces el juez declara que está acorralado
    if (!escape) {tablero.puma_capturado = true;}

    return false; // False, no se movió al puma
}

bool condiciones_victoria(Tablero& tablero, int id_partida){
    
    // Puma se come a 6 perros
    if (tablero.perros_capturados >= 6) {
        tablero.partida_activa = false;
        cout << "[Partida " << id_partida << "] finaliza con victoria del Puma con " << tablero.perros_capturados << " perros capturados\n";
        tablero.archivo_log << "[Partida " << id_partida << "] finaliza con victoria del Puma con " << tablero.perros_capturados << " perros capturados\n";
        return true;
    }

    if (tablero.puma_capturado) {
        tablero.partida_activa = false;
        cout << "[Partida " << id_partida << "] finaliza con victoria de Perros por acorralar al Puma\n";
        tablero.archivo_log << "[Partida " << id_partida << "] finaliza con victoria de Perros por acorralar al Puma\n";
        return true;
    }

    return false; // Nadie ha ganado, partida sigue activa
}

bool verificar_jugadores_activos(Tablero& tablero, int id_partida) {
    // Revisar si el CATTO respondió (si sigue en true, no respondió y abandonó)
    if (tablero.verificar_conexion[12]) {
        tablero.partida_activa = false;
        cout << "[Partida " << id_partida << "] ABANDONO: Puma le dió la pera y dejó de responder. Victoria de Perros por abandono\n";
        tablero.archivo_log << "[Partida " << id_partida << "] ABANDONO: Puma le dió a pera y dejó de responder. Victoria de Perros por abandono\n";
        return true;
    }
    // Si respondió (estaba en false), el juez vuelve a dejarlo en true para la próxima consulta
    tablero.verificar_conexion[12] = true;

    // Revisar solo a los perros watones que siguen vivos
    for (int i = 0; i < 12; i++) {
        if (tablero.perro_vivo[i]) {
            if (tablero.verificar_conexion[i]) {
                tablero.partida_activa = false;
                cout << "[Partida " << id_partida << "] ABANDONO: Perro " << i << " se le hizo y dejó de responder. Victoria del Puma por abandono\n";
                tablero.archivo_log << "[Partida " << id_partida << "] ABANDONO: Perro " << i << " se le hizo dejó de responder. Victoria del Puma por abandono\n";
                return true;
            }
            // Si respondió (estaba en false), el juez vuelve a dejarlo en true para la próxima consulta
            tablero.verificar_conexion[i] = true;
        }
    }

    return false; // Todos los hilos vivos siguen respondiendo
}

void* rutina_mesa(void* arg) {
    Mesa* mesa = (Mesa*)arg;

    cout << "[Partida " << mesa->id_partida << "] Inicio de partida\n";
    
    Tablero tablero;
    iniciar_tablero(tablero, mesa->config); // la mesa se encarga de inicializar el trablero
    tablero.id_partida = mesa->id_partida;

    // Abrir archivo de log separado para esta partida
    string nombre_log = "partida_" + to_string(mesa->id_partida) + ".log";
    tablero.archivo_log.open(nombre_log);
    tablero.archivo_log << "[Partida " << mesa->id_partida << "] Inicio de partida\n";
    tablero.archivo_log << "Estado inicial del tablero:\n";
    guardar_tablero_log(tablero);
    
    // variables de los 13 hilos (dogis y cat) para lanzarlos 
    pthread_t hebra_puma;
    pthread_t hebras_perros[12];
    Jugador datos_puma;  
    Jugador datos_perros[12];

    iniciar_jugadores(tablero, mesa->config, mesa->id_partida, hebra_puma, hebras_perros, datos_puma, datos_perros);

    int crono_ms = 0;
    int tiempo_limite = mesa->config.delay_max * 2 + 100; // Margen para que todas las fichas vivas alcancen a despertar y responder

    while (true) {
        usleep(1 * 1000); // pregunta cada 1 ms para atender rápido la cola
        crono_ms++;
        
        pthread_mutex_lock(&tablero.lock_tablero); // bloquea el tablero para procesar peticiones

        // El juez procesa todas las peticiones de movimiento que llegaron a la cola
        for (int i = 0; i < (int)tablero.cola_peticiones.size(); i++) {

            Peticion p = tablero.cola_peticiones[i];
            if (p.tipo == PERRO) {

                validar_movimiento_perro(tablero, p.id_pieza, p.destino);

            } else if (p.tipo == PUMA) {
                validar_movimiento_puma(tablero, p.destino);
            }

            tablero.peticion_pendiente[p.id_pieza] = false;

            if (condiciones_victoria(tablero, mesa->id_partida)) {
                break;
            }
        }

        // Limpiar la cola después de atender las peticiones
        tablero.cola_peticiones.clear();

        // Si aún nadie ha ganado y se cumplió el tiempo del cronómetro, el juez verifica que nadie haya abandonado
        if (tablero.partida_activa && crono_ms >= tiempo_limite) {
            crono_ms = 0;
            verificar_jugadores_activos(tablero, mesa->id_partida);
        }

        // Revisa si alguien ha ganado o si la partida terminó por abandono
        if (!tablero.partida_activa) {

            pthread_mutex_unlock(&tablero.lock_tablero);
            break; // Si alguien gana, termino la partida (sale el hilo del ciclo while)
        }

        pthread_mutex_unlock(&tablero.lock_tablero); // Desbloquea tablero para que el juego continúe
    }

    // Esperar a que hebras terminen (después de que se haya salido del ciclo while, es decir, una vez que alguien haya ganado)
    pthread_join(hebra_puma, NULL);

    for (int i = 0; i < 12 ; i++) {
        pthread_join(hebras_perros[i], NULL);
    } 

    pthread_mutex_destroy(&tablero.lock_tablero);
    sem_post(mesa->max_mesas);

    cout << "[Partida " << mesa->id_partida << "] terminó, mesa liberada\n";
    tablero.archivo_log << "[Partida " << mesa->id_partida << "] terminó, mesa liberada\n";
    tablero.archivo_log.close();
    return NULL;
}
