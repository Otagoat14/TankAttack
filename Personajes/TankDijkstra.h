#ifndef TANKDIJKSTRA_H
#define TANKDIJKSTRA_H

#include "Tank.h"
#include "../Utils.h"
#include "../str/Grafo.h"

class tankDijkstra : public Tank {
public:
    tankDijkstra(int x, int y, int vida, Equipo equipo, Color color);
    void moverse(int nx, int ny, Graph& grafo) override;

private:
    Camino Dijkstra(Graph& grafo, Posicion inicio, Posicion destino);
    bool explorarDijkstra(Graph& grafo, ColaPrioridad& pq, int* distancias, int* padre, bool* visitado, int nodoDestino);
};

#endif