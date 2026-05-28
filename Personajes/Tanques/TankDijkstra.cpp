#include "TankDijkstra.h"
#include <iostream>
#include <climits>

using namespace std;

// Constructor
tankDijkstra::tankDijkstra(int x, int y, int vida, Equipo equipo, Color color)
    : Tank(x, y, vida, equipo, color) {}


void tankDijkstra::moverse(int nx, int ny, Graph& grafo, Map& map, bool altaPrecision) {
    int umbral   = altaPrecision ? 90 : 80; // Probabilidad de usar Dijkstra, 90 power up, 80 sin
    int decision = rand() % 100;            // Numero aleatorio entre 0 y 99

    if (decision < umbral) {
        //Movimiento con Dijkstra
        Posicion inicio  = {getY(), getX()}; // Posicion actual {fila, columna}
        Posicion destino = {ny, nx};         // Celda destino

        Camino camino = Dijkstra(grafo, map, inicio, destino);

        if (camino.empty()) {
            cout << "Dijkstra: No hay camino" << endl;
            return;
        }

        // iniciarMovimiento recibe un Camino* en heap, Tank se encarga de liberarlo
        iniciarMovimiento(new Camino(camino));

    } else {
        //Movimiento aleatorio
        Camino* camino = new Camino(
            celdаsLineaVista(getX(), getY(), nx, ny, grafo)
        );
        if (camino->empty()) {
            delete camino;
            return;
        }
        iniciarMovimiento(camino);
    }
}


Camino tankDijkstra::Dijkstra(Graph& grafo, Map& map, Posicion inicio, Posicion destino) {
    int totalNodos = grafo.getTotalNodos();

    // Arreglos en heap porque el tamano depende del mapa
    int*  distancias = new int[totalNodos];
    int*  padre      = new int[totalNodos];
    bool* visitado   = new bool[totalNodos];

    // Inicializar: distancia infinita (INT_MAX)
    for (int i = 0; i < totalNodos; i++) {
        distancias[i] = INT_MAX;
        padre[i]      = -1;
        visitado[i]   = false;
    }

    int nodoInicio  = grafo.getNodo(inicio.r, inicio.c);
    int nodoDestino = grafo.getNodo(destino.r, destino.c);

    // El nodo de inicio tiene costo 0 para llegar a si mismo
    distancias[nodoInicio] = 0;

    // Siempre extrae el nodo de menor costo acumulado.
    ColaPrioridad pq;
    pq.insertar(nodoInicio, 0);

    bool encontrado = explorarDijkstra(grafo, map, pq, distancias, padre, visitado, nodoDestino);

    // Reconstruir el camino usando padre[]
    Camino camino;
    if (encontrado)
        camino = reconstruirCamino(grafo, padre, nodoDestino);

    // Liberar memoria de los arreglos temporales
    delete[] distancias;
    delete[] padre;
    delete[] visitado;
    return camino;
}

bool tankDijkstra::explorarDijkstra(Graph& grafo, Map& map, ColaPrioridad& pq,
                                     int* distancias, int* padre,
                                     bool* visitado, int nodoDestino) {
    while (!pq.empty()) {
        int actual = pq.extraerMin(); // Siempre el nodo de menor costo acumulado

        if (actual == nodoDestino)
            return true; // Encontrado con el costo minimo garantizado

        // Saltar si ya fue procesado definitivamente
        if (visitado[actual])
            continue;
        visitado[actual] = true;

        // Explorar vecinos transitables
        int vecinos[4];
        int numVecinos = 0;
        grafo.getVecinos(actual, vecinos, numVecinos);

        for (int i = 0; i < numVecinos; i++) {
            int vecino = vecinos[i];
            if (visitado[vecino]) continue; // Ya procesado definitivamente

            // Obtener el peso real del terreno desde el mapa
            int peso = map.getPeso(vecino);
            if (peso == -1) continue; // Es un obstaculo, no se puede pasar

            // Calcular nuevo costo para llegar al vecino por este camino
            int nuevoCosto = distancias[actual] + peso;

            // Solo actualizar si encontramos un camino mas barato al vecino
            if (nuevoCosto < distancias[vecino]) {
                distancias[vecino] = nuevoCosto;
                padre[vecino]      = actual; // Registrar quien lo descubrio con menor costo
                pq.insertar(vecino, nuevoCosto); // Agregar con nuevo costo a la cola
            }
        }
    }
    return false; // Cola vacia, no hay camino al destino
}