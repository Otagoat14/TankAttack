#ifndef TANKDIJKSTRA_H
#define TANKDIJKSTRA_H

#include "Tank.h"
#include "../../Utils.h"
#include "../../str/Grafo.h"
#include "../../str/Map.h"

class tankDijkstra : public Tank {
public:
    tankDijkstra(int x, int y, int vida, Equipo equipo, Color color);
    void moverse(int nx, int ny, Graph& grafo, Map& map, bool altaPrecision = false) override;

private:
    Camino Dijkstra(Graph& grafo, Map& map, Posicion inicio, Posicion destino);
    bool explorarDijkstra(Graph& grafo, Map& map, ColaPrioridad& pq,
                          int* distancias, int* padre, bool* visitado, int nodoDestino);
};

#endif