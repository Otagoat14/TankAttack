//
// Created by nacho on 5/11/2026.
//

#ifndef TANKATTACK_MAPRENDER_H
#define TANKATTACK_MAPRENDER_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Grafo.h"


class MapRender {
private:
    Map& mapa;
    Graph& grafo;

    //Tamano de cada celda
    int cellSize;
    //Origen relativo del mapa en la ventana en x
    int offsetX;
    //Origen relativo del mapa en la ventana en y
    int offsetY;
    //Ancho del mapa en pixeles
    int anchoMapa;
    //Alto del mapa en pixeles
    int altoMapa;
    //Columnas
    int colm;
    //Filas
    int filas;

    struct DatosCelda
    {
        sf::RectangleShape base;
        bool seleccionado;
    };

    DatosCelda* celda;

    void construirMapa();

    sf::Color getColor(int peso);

public:
    //Constructor
    MapRender(Map& mapa, Graph& grafo, int offsetX, int offsetY, int areaW, int areaH);
    //Destructor
    ~MapRender();

    //Obtiene mediante pixeles que celda es
    sf::Vector2i obtenerCelda( int px, int py);

    //Obtiene las coordenadas en pixeles del centro de la celda
    sf::Vector2f obtenerCentro( int fila, int colm);

    //Dibuja el mapa
    void dibujar(sf::RenderWindow& ventana);

};


#endif //TANKATTACK_MAPRENDER_H