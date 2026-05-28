#include "TankBFS.h"
#include <iostream>

using namespace std;

// Constructor
tankBFS::tankBFS(int x, int y, int vida, Equipo equipo, Color color)
    : Tank(x, y, vida, equipo, color) {}


void tankBFS::moverse(int nx, int ny, Graph& grafo, Map& map, bool altaPrecision) {
    int umbral   = altaPrecision ? 90 : 50; // Probabilidad de usar BFS, 90 con el power up, 50 sin
    int decision = rand() % 100;            // Numero aleatorio entre 0 y 99

    if (decision < umbral) {
        //Movimiento con BFS
        Posicion inicio  = {getY(), getX()}; // Posicion actual del tanque {fila, columna}
        Posicion destino = {ny, nx};         // Celda destino clickeada por el jugador

        Camino camino = BFS(grafo, inicio, destino);

        if (camino.empty()) {
            cout << "BFS: No hay camino" << endl;
            return; // No se encontro ruta, el tanque no se mueve
        }

        // iniciarMovimiento recibe un Camino* en heap, Tank se encarga de liberarlo
        iniciarMovimiento(new Camino(camino));

    } else {
        //Movimiento aleatorio
        Camino* camino = new Camino(
            celdаsLineaVista(getX(), getY(), nx, ny, grafo)
        );
        if (camino->empty()) {
            delete camino; // Liberar si no hay camino para evitar memory leak
            return;
        }
        iniciarMovimiento(camino);
    }
}
Camino tankBFS::BFS(Graph& grafo, Posicion inicio, Posicion destino) {
    int totalNodos = grafo.getTotalNodos();

    // Arreglos en heap porque el tamano depende del mapa
    bool* visitado = new bool[totalNodos];
    int*  padre    = new int[totalNodos];

    // Inicializar
    for (int i = 0; i < totalNodos; i++) {
        visitado[i] = false;
        padre[i]    = -1;
    }

    // Convertir coordenadas a indice unico del grafo
    int nodoInicio  = grafo.getNodo(inicio.r, inicio.c);
    int nodoDestino = grafo.getNodo(destino.r, destino.c);

    Cola<int> cola;
    cola.enqueue(nodoInicio);
    visitado[nodoInicio] = true;

    // Explorar el grafo y llenar el arreglo padre[]
    bool encontrado = explorarBFS(grafo, cola, visitado, padre, nodoDestino);

    // Reconstruir el camino usando padre[]
    Camino camino;
    if (encontrado)
        camino = reconstruirCamino(grafo, padre, nodoDestino);

    // Liberar memoria de los arreglos temporales
    delete[] visitado;
    delete[] padre;
    return camino;
}


bool tankBFS::explorarBFS(Graph& grafo, Cola<int>& cola, bool* visitado, int* padre, int nodoDestino) {
    while (!cola.empty()) {
        int actual = cola.dequeue(); // Sacar el primer nodo de la cola
        cout << "Explorando nodo : " << actual << endl;

        if (actual == nodoDestino)
            return true; // Encontrado

        // Obtener los vecinos transitables del nodo actual
        int vecinos[4];
        int numVecinos = 0;
        grafo.getVecinos(actual, vecinos, numVecinos);

        for (int i = 0; i < numVecinos; i++) {
            if (!visitado[vecinos[i]]) {
                visitado[vecinos[i]] = true;
                padre[vecinos[i]]    = actual; // Registrar quien descubrio este vecino
                cola.enqueue(vecinos[i]);      // Agregar a la cola para explorar despues
            }
        }
    }
    return false; // Se vacio la cola sin encontrar el destino
}