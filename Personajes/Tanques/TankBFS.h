#ifndef TANKBFS_H
#define TANKBFS_H

#include "Tank.h"
#include "../../Utils.h"
#include "../../str/Grafo.h"
#include "../../str/Map.h"

class tankBFS : public Tank {
public:
    tankBFS(int x, int y, int vida, Equipo equipo, Color color);
    void moverse(int nx, int ny, Graph& grafo, Map& map) override;

private:
    Camino BFS(Graph& grafo, Posicion inicio, Posicion destino);
    bool explorarBFS(Graph& grafo, Cola<int>& cola, bool* visitado, int* padre, int nodoDestino);
};

#endif