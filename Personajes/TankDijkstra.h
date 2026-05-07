#ifndef TANKDIJKSTRA_H
#define TANKDIJKSTRA_H
#include "Tank.h"
#include "../Utils.h"
#include "../Grafo.h"




class tankDijkstra : public Tank {
public:
    tankDijkstra(int x, int y, int vida, Equipo equipo, Color color);
    void moverse(vector<vector<int>>& matriz) override;

    vector<Posicion> Dijkstra(Graph& grafo, PosicionD inicio, PosicionD destino);
};

#endif