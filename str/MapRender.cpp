#include "MapRender.h"
#include <iostream>

MapRender::MapRender(Map& mapa, Graph& grafo,
                     int offsetX, int offsetY,
                     int areaX, int areaY)
    : grafo(grafo), mapa(mapa)
{
    this->filas = grafo.getRows();
    this->colm  = grafo.getCols();

    celda = new DatosCelda[filas * colm];

    cellSize = std::min(
        areaX / grafo.getCols(),
        areaY / grafo.getRows()
    );

    anchoMapa = cellSize * grafo.getCols();
    altoMapa  = cellSize * grafo.getRows();

    this->offsetX = offsetX + (areaX - anchoMapa) / 2;
    this->offsetY = offsetY + (areaY - altoMapa)  / 2;

    cargarTexturas();
    construirMapa();
}

MapRender::~MapRender() {
    delete[] celda;
}

// ─────────────────────────────────────────────────────────────────────────────
// Carga las texturas desde Interfaz/Sprites/
// La ruta es relativa al ejecutable (cmake-build-debug/)
// ─────────────────────────────────────────────────────────────────────────────
void MapRender::cargarTexturas() {
    texturasCargadas = true;

    //Ruta texturas
    const std::string base = "Sprites/";

    if (!texCesped.loadFromFile(base + "Cesped.png")) {
        std::cerr << "[MapRender] No se pudo cargar Cesped.png\n";
        texturasCargadas = false;
    }
    if (!texBarro.loadFromFile(base + "Barro.png")) {
        std::cerr << "[MapRender] No se pudo cargar Barro.png\n";
        texturasCargadas = false;
    }
    if (!texAgua.loadFromFile(base + "Agua.png")) {
        std::cerr << "[MapRender] No se pudo cargar Agua.png\n";
        texturasCargadas = false;
    }
    if (!texMontain.loadFromFile(base + "Montain.png")) {
        std::cerr << "[MapRender] No se pudo cargar Montain.png\n";
        texturasCargadas = false;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Devuelve la textura correspondiente al peso
// ─────────────────────────────────────────────────────────────────────────────
sf::Texture& MapRender::getTextura(int peso) {
    switch (peso) {
        case  1: return texCesped;
        case  2: return texBarro;
        case  5: return texAgua;
        case -1: return texMontain;
        default: return texCesped;   // fallback
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Construye el array de sprites, uno por celda
// ─────────────────────────────────────────────────────────────────────────────
void MapRender::construirMapa() {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < colm; j++) {
            int cel  = i * colm + j;
            int nodo = grafo.getNodo(i, j);
            int peso = mapa.getPeso(nodo);

            float x = offsetX + j * cellSize;
            float y = offsetY + i * cellSize;

            sf::Texture& tex = getTextura(peso);

            // Calcula la escala para que el sprite llene exactamente la celda
            float scaleX = (float)cellSize / tex.getSize().x;
            float scaleY = (float)cellSize / tex.getSize().y;

            celda[cel].sprite.setTexture(tex);
            celda[cel].sprite.setScale(scaleX, scaleY);
            celda[cel].sprite.setPosition(x, y);
            celda[cel].seleccionado = false;
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Utilidades
// ─────────────────────────────────────────────────────────────────────────────
float const MapRender::getCellSize(){return cellSize;}

sf::Vector2i MapRender::obtenerCelda(int px, int py) {
    int fila    = (py - offsetY) / cellSize;
    int columna = (px - offsetX) / cellSize;
    return sf::Vector2i(columna, fila);
}

sf::Vector2f MapRender::obtenerCentro(int fila, int colm) {
    int px = offsetX + colm * cellSize + (cellSize / 2);
    int py = offsetY + fila * cellSize + (cellSize / 2);
    return sf::Vector2f(px, py);
}

bool MapRender::encontrarPosLibre(int colInicio, int filaInicio, bool desdeIzq, int& colResult, int& filaResult) {
    for (int f = filaInicio; f < filas; f++) {
        if (desdeIzq) {
            for (int c = colInicio; c < colInicio + 3; c++) {
                int nodo = grafo.getNodo(f, c);
                if (mapa.getPeso(nodo) != -1) {
                    colResult = c; filaResult = f; return true;
                }
            }
        } else {
            for (int c = colInicio; c > colInicio - 3; c--) {
                int nodo = grafo.getNodo(f, c);
                if (mapa.getPeso(nodo) != -1) {
                    colResult = c; filaResult = f; return true;
                }
            }
        }
    }
    return false;
}
// ─────────────────────────────────────────────────────────────────────────────
// Dibujo
// ─────────────────────────────────────────────────────────────────────────────
void MapRender::dibujar(sf::RenderWindow& ventana) {
    for (int i = 0; i < filas * colm; i++) {
        ventana.draw(celda[i].sprite);
    }
};