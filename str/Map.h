//
// Created by nacho on 5/8/2026.
//

#ifndef TANKATTACK_MAP_H
#define TANKATTACK_MAP_H

#include "Grafo.h"

class Map
{
private:
    int* pesos;
    int rows;
    int cols;
    int numObstaculos;
    Graph& grafo;

public:
    Map(Graph& grafo, int rows, int cols, int porcentaje);
    ~Map();

    //Funcion Encargada de la generacion de el mapa
    void generateMap();

    //Funcion encargada de obtenerel peso de un nodo
    int getPeso(int nodo);
};

#endif //TANKATTACK_MAP_H