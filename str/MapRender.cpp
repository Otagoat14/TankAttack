//
// Created by nacho on 5/11/2026.
//

#include "MapRender.h"

MapRender::MapRender(Map& mapa, Graph& grafo, int offsetX, int offsetY, int areaX, int areaY)
:grafo(grafo), mapa(mapa)
{
    this->filas = grafo.getRows();
    this->colm = grafo.getCols();

    //
    celda = new DatosCelda[filas * colm];

    cellSize = std::min(
        areaX / grafo.getCols(),
        areaY / grafo.getRows()
    );

    anchoMapa = cellSize * grafo.getCols();
    altoMapa = cellSize * grafo.getRows();
                    //offset inicial sin ajuste para centrar el mapa
    //resultado offset que contempla el debelance resultante de los pixeles sobrantes
    this->offsetX = offsetX + (areaX - anchoMapa)/ 2;
    this->offsetY = offsetY + (areaY - altoMapa)/ 2;
    construirMapa();
}


MapRender::~MapRender(){ delete[] celda;}

sf::Color MapRender::getColor(int peso)
{
    switch(peso) {
        case -1: return sf::Color(38, 38, 38);
        case  1: return sf::Color(135, 161, 88);
        case  2: return sf::Color(88, 65, 28);
        case  5: return sf::Color(28, 65, 105);
        default: return sf::Color::White;
    }
}

sf::Vector2i MapRender::obtenerCelda(int px, int py)
{
    int fila = (py - offsetY)/cellSize;
    int columna = (px - offsetX)/cellSize;
    return sf::Vector2i(columna, fila);
}

sf::Vector2f MapRender::obtenerCentro(int fila, int colm)
{
    int px = offsetX + colm * cellSize + (cellSize / 2);
    int py = offsetY + fila * cellSize + (cellSize / 2);
    return sf::Vector2f(px, py);
}

void MapRender::construirMapa() {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < colm; j++) {
            int cel = i * colm + j;
            int nodo = grafo.getNodo(i, j);
            int peso = mapa.getPeso(nodo);

            float x = offsetX + j * cellSize;
            float y = offsetY + i * cellSize;

            celda[cel].base.setSize(sf::Vector2f(cellSize, cellSize));
            celda[cel].base.setPosition(sf::Vector2f(x, y));
            celda[cel].base.setFillColor(getColor(peso));
            celda[cel].seleccionado = false;
        }
    }
}

void MapRender::dibujar(sf::RenderWindow& ventana) {
    for (int i = 0; i < filas * colm; i++) {
        ventana.draw(celda[i].base);
    }
}