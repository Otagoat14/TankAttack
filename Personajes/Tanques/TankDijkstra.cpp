#include "TankDijkstra.h"
#include <iostream>
#include <climits>

using namespace std;

tankDijkstra::tankDijkstra(int x, int y, int vida, Equipo equipo, Color color)
    : Tank(x, y, vida, equipo, color) {}

void tankDijkstra::moverse(int nx, int ny, Graph& grafo, Map& map) {
    // 80% Dijkstra, 20% aleatorio
    int decision = rand() % 100;

    if (decision < 80) {
        // Dijkstra
        Posicion inicio  = {getY(), getX()};
        Posicion destino = {ny, nx};
        Camino camino = Dijkstra(grafo, map, inicio, destino);

        if (camino.empty()) {
            cout << "Dijkstra: No hay camino" << endl;
            return;
        }
        iniciarMovimiento(new Camino(camino));

    } else {
        // Movimiento aleatorio
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

    int*  distancias = new int[totalNodos];
    int*  padre      = new int[totalNodos];
    bool* visitado   = new bool[totalNodos];

    for (int i = 0; i < totalNodos; i++) {
        distancias[i] = INT_MAX;
        padre[i]      = -1;
        visitado[i]   = false;
    }

    int nodoInicio  = grafo.getNodo(inicio.r, inicio.c);
    int nodoDestino = grafo.getNodo(destino.r, destino.c);

    distancias[nodoInicio] = 0;

    ColaPrioridad pq;
    pq.insertar(nodoInicio, 0);

    bool encontrado = explorarDijkstra(grafo, map, pq, distancias, padre, visitado, nodoDestino);

    Camino camino;
    if (encontrado)
        camino = reconstruirCamino(grafo, padre, nodoDestino);

    delete[] distancias;
    delete[] padre;
    delete[] visitado;
    return camino;
}

bool tankDijkstra::explorarDijkstra(Graph& grafo, Map& map, ColaPrioridad& pq, int* distancias, int* padre, bool* visitado, int nodoDestino) {
    while (!pq.empty()) {
        int actual = pq.extraerMin();

        if (actual == nodoDestino)
            return true;

        if (visitado[actual])
            continue;
        visitado[actual] = true;

        int vecinos[4];
        int numVecinos = 0;
        grafo.getVecinos(actual, vecinos, numVecinos);

        for (int i = 0; i < numVecinos; i++) {
            int vecino = vecinos[i];
            if (visitado[vecino]) continue;

            // Usar peso real del mapa
            int peso = map.getPeso(vecino);
            if (peso == -1) continue;  // obstaculo, saltar

            int nuevoCosto = distancias[actual] + peso;

            if (nuevoCosto < distancias[vecino]) {
                distancias[vecino] = nuevoCosto;
                padre[vecino]      = actual;
                pq.insertar(vecino, nuevoCosto);
            }
        }
    }
    return false;
}