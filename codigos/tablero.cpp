#include <vector>
using namespace std;
// Representacion del contenido de cada casilla
enum EstadoCasilla { VACIA, PUMA, PERRO };

// Estructura de un nodo en el grafo
struct Casilla {
    int id;
    EstadoCasilla estado;
    vector<int> adyacentes; // ids de las casillas
    
    // aqui mas adelante se agregara un mutex para bloquear casilla por casilla
};

class Tablero{
private:
    vector<Casilla> grafo; // vector de structs de tipo Casilla o nodo

public:

}

