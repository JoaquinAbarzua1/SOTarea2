# Tarea 2 - Torneo de Komikan Concurrente

## Integrantes
* Joaquín Abarzúa
* Pedro Josué Rojas

## 1. Compilar
Escribir en la terminal (dentro de la carpeta `SOTarea2/`, donde está el `Makefile`):
```
make
```

Esto compila todos los archivos de `codigos/` y genera el binario `torneo`.

Para borrar el binario y limpiar los archivos `.log` de ejecuciones anteriores:
```
make clean
```

## 2. Ejecutar

Sin archivo de configuración (usa los valores por defecto):
```
./torneo N M
```

Con archivo de configuración (ubicado en la misma carpeta que el `Makefile`):
```
./torneo N M config.txt
```

* **`N`**: Cantidad total de jugadores (entero par y positivo). Como se emparejan de a 2 por mesa (1 jugador controla al Puma y 1 jugador controla a los Perros), se juegan **`N / 2` partidas** en total.
* **`M`**: Cantidad máxima de mesas activas al mismo tiempo (entero positivo y menor o igual a `N / 2`).
* **`config.txt`** (opcional): Define la semilla, los retardos de movimiento y las posiciones iniciales de las fichas.

### Formato de `config.txt` y valores por defecto
Las coordenadas siguen siempre el formato **`fila columna`** (`y x`). Ejemplo de `config.txt`:

```
SEED 67
DELAY_MIN 50
DELAY_MAX 300
PUMA_INIT 0 2
DOG_INIT 6 0 6 1 6 2 6 3 6 4 5 0 5 1 5 2 5 3 5 4 4 0 4 4
```

Si no se pasa `config.txt` (o si se omiten parámetros en el archivo), el programa usa esos mismos valores por defecto (`SEED = 67`, `DELAY_MIN = 50` ms, `DELAY_MAX = 300` ms, el Puma en `[0, 2]` y los 12 perros en las filas 4, 5 y 6). Si se definen posiciones iniciales en el archivo, deben especificarse tanto `PUMA_INIT` como los 12 perros en `DOG_INIT`.

## 3. Explicación del Funcionamiento

### Estructura de los archivos

Para no tener un `main.cpp` gigante, la lógica está separada en módulos dentro de `codigos/`:

* **`parseo.hpp` / `parseo.cpp`**: Lee `config.txt`, verifica que los parámetros y coordenadas sean válidos (por ejemplo, que nadie parta en `(1, 2)` que es solo un cruce de líneas) y rellena el struct `ConfigTorneo`.
* **`tablero.hpp` / `tablero.cpp`**: Define el struct `Tablero` (la matriz `7x5`, las posiciones del puma y los perros, el mutex `lock_tablero` y la cola de peticiones), la tabla de adyacencias de los 28 nodos del grafo y las funciones para guardar el estado del tablero en el log.
* **`jugadores.hpp` / `jugadores.cpp`**: Define el struct `Jugador` (los datos que recibe cada hilo de ficha) y la función `iniciar_jugadores` que lanza los 13 hilos por partida (1 Puma y 12 Perros).
* **`perro.cpp`**: Contiene el comportamiento de cada perro (`rutina_perro` y `mover_perro`).
* **`puma.cpp`**: Contiene el comportamiento del puma (`rutina_puma` y `mover_puma`).
* **`mesa.hpp` / `mesa.cpp`**: Contiene la lógica del Juez de cada partida (`rutina_mesa`), que valida los movimientos y saltos de captura, detecta quién ganó o si alguien abandonó, y escribe en el archivo `.log` de la partida.
* **`main.cpp`**: Valida los argumentos `N` y `M`, maneja la señal `SIGINT` (`Ctrl+C`), y funciona como Matchmaker usando el semáforo `max_mesas` para lanzar las `N/2` partidas sin superar las `M` mesas simultáneas.

### Sincronización y ciclo de los hilos

Para evitar condiciones de carrera dentro de una partida, cada mesa tiene su propio mutex (`tablero.lock_tablero`) que protege el tablero y una cola de peticiones (`cola_peticiones`).

#### Ciclo de las fichas (Puma y Perros):
1. El hilo duerme un tiempo aleatorio (`usleep`) entre `delay_min` y `delay_max` **fuera del mutex**, y elige al azar una casilla vecina usando `rand_r`.
2. Agarra el mutex `tablero.lock_tablero`.
3. Responde al chequeo de conexión del Juez (`verificar_conexion[id] = false`) para avisar que sigue activo.
4. Si la partida sigue activa y la ficha no tiene otra petición pendiente, mete su solicitud de movimiento en `tablero.cola_peticiones` y marca `peticion_pendiente[id] = true`.
5. Suelta el mutex para que otras fichas o el Juez puedan usarlo.

#### Ciclo del Juez (`rutina_mesa`):
1. Cada mesa corre en su propio hilo y crea sus 13 hilos de fichas.
2. En su ciclo, agarra `tablero.lock_tablero`, revisa todas las solicitudes acumuladas en `cola_peticiones` en orden de llegada y valida si la casilla destino sigue libre (o si el Puma puede saltar y comer a un perro).
3. Después de cada movimiento válido, imprime el evento en la terminal y dibuja la matriz `7x5` completa en el archivo **`partida_X.log`** (generado en la misma carpeta donde se ejecuta `./torneo`).
4. Revisa las condiciones de victoria (Puma captura 6 perros o Perros dejan al Puma acorralado sin movimientos) y periódicamente revisa que ninguna ficha viva haya dejado de responder (abandono).
5. Cuando la partida termina (o si se presiona `Ctrl+C`), pone `partida_activa = false`, suelta el mutex, espera a los 13 hilos con `pthread_join`, destruye el mutex, cierra `partida_X.log` y libera el cupo en el semáforo (`sem_post`) para que entre otra partida.

