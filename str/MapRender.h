#ifndef TANKATTACK_MAPRENDER_H
#define TANKATTACK_MAPRENDER_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Grafo.h"

class MapRender {
private:
    Map& mapa;
    Graph& grafo;

    int cellSize;  // Tamaño en píxeles de cada celda
    int offsetX;   // Desplazamiento horizontal para centrar el mapa
    int offsetY;   // Desplazamiento vertical para centrar el mapa
    int anchoMapa; // Ancho total del mapa en píxeles
    int altoMapa;  // Alto total del mapa en píxeles
    int colm;      // Número de columnas del mapa
    int filas;     // Número de filas del mapa

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

    DatosCelda* celda; // Arreglo de sprites, uno por celda del mapa

    //Carga las 4 texturas de terreno
    void cargarTexturas();
    // Construye el arreglo de sprites asignando textura, escala y posición a cada celda
    void construirMapa();
    // Retorna la textura correspondiente al peso del nodo
    sf::Texture& getTextura(int peso);   // devuelve la textura según el peso

public:
    //Inicializa el renderer calculando dimensiones, centrando el mapa,
    //cargando la textura y cargando los sprites
    MapRender(Map& mapa, Graph& grafo,
              int offsetX, int offsetY,
              int areaW, int areaH);
    // Libera el arreglo de celdas
    ~MapRender();

    // Retorna el tamaño en píxeles de cada celda
    float const getCellSize();

    // Convierte coordenadas de pantalla a coordenadas del grafo (fila,columna)
    sf::Vector2i obtenerCelda(int px, int py);
    // Retorna la posición en píxeles del centro de la celda (fila, colm)
    sf::Vector2f obtenerCentro(int fila, int colm);
    // Dibuja todos los sprites del mapa en la ventana
    void dibujar(sf::RenderWindow& ventana);
    // Busca la pprimera celda trancitabole desde la columna de inicio
    bool encontrarPosLibre(int colInicio, int filaInicio, bool desdeIzq, int& colResult, int& filaResult);
};

#endif