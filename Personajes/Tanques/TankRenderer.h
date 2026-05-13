//
// Created by nacho on 5/12/2026.
//

#ifndef TANKATTACK_TANKRENDERER_H
#define TANKATTACK_TANKRENDERER_H

#include "Tank.h"
#include "TankBFS.h"
#include "TankDijkstra.h"
#include "C:\Datos 2\TankAttack\str\MapRender.h"
#include "SFML/Graphics/Rect.hpp"

int inline spriteAlto = 189;
int inline spriteAncho = 331;

class TankRenderer
{
    private:
    sf::Texture texturas[4];
    sf::Sprite sprites[4];
    MapRender& mapaRender;

    //Le da un indice a cada color para facilitar la asignacion de sprites
    int giveColor(Color color);
    //Calcula que parte de la imegen debe proyectar
    int getColumna(Direccion dir);
    //Busca el sprite a proyectar en el PNG
    sf::IntRect calcPosicion(Direccion dir);

public:
    TankRenderer(MapRender& mapaRender);
    ~TankRenderer();

    void dibujarTanques(sf::RenderWindow& ventana, Tank** tanques, int numTanques);

};


#endif //TANKATTACK_TANKRENDERER_H