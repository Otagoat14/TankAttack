#ifndef TANKATTACK_MAPRENDER_H
#define TANKATTACK_MAPRENDER_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Grafo.h"

class MapRender {
private:
    Map& mapa;
    Graph& grafo;

    int cellSize;
    int offsetX;
    int offsetY;
    int anchoMapa;
    int altoMapa;
    int colm;
    int filas;

    // ── Texturas (una por tipo de terreno) ──
    sf::Texture texCesped;    // peso  1
    sf::Texture texBarro;     // peso  2
    sf::Texture texAgua;      // peso  5
    sf::Texture texMontain;   // peso -1
    bool        texturasCargadas;

    struct DatosCelda {
        sf::Sprite  sprite;
        bool        seleccionado;
    };

    DatosCelda* celda;

    void cargarTexturas();
    void construirMapa();
    sf::Texture& getTextura(int peso);   // devuelve la textura según el peso

public:
    MapRender(Map& mapa, Graph& grafo,
              int offsetX, int offsetY,
              int areaW, int areaH);
    ~MapRender();

    float const getCellSize();

    sf::Vector2i obtenerCelda(int px, int py);
    sf::Vector2f obtenerCentro(int fila, int colm);
    void dibujar(sf::RenderWindow& ventana);
    bool encontrarPosLibre(int colInicio, int filaInicio, bool desdeIzq, int& colResult, int& filaResult);
};

#endif