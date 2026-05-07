
#ifndef TANKBFS_H
#define TANKBFS_H
#include <iostream>
#include "../Utils.h"
#include "Tank.h"
#include <vector>


class tankBFS : public Tank {
public:
    tankBFS(int x, int y, int vida, Equipo equipo, Color color);
    void moverse(vector<vector<int>>& matriz) override;


    //Remplazar el vector con el grafo
    vector<Posicion> BFS(vector<vector<int>>& matriz, Posicion inicio, Posicion final);
};


#endif
