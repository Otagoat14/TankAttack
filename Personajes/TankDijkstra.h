//
// Created by otago on 5/2/2026.
//

#ifndef TANKDIJKSTRA_H
#define TANKDIJKSTRA_H
#include "Tank.h"

class tankDijkstra : public Tank {
public:
    tankDijkstra(int x, int y, int vida, Equipo equipo, Color color);
    void moverse(vector<vector<bool>>& matriz) override;
};

#endif 
