
#ifndef TANKBFS_H
#define TANKBFS_H
#include "../Cola.h"
#include "Tank.h"

class tankBFS : public Tank {
public:
    tankBFS(int x, int y, int vida, Equipo equipo, Color color);
    void moverse(vector<vector<bool>>& matriz) override;

    //Ya la cola fue implementada, queda implementarla con el grafo
    void BFS(int inicio, vector<vector<int>>& adj, int numVertices);
};


#endif
