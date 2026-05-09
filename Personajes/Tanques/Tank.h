#ifndef TANK_H
#define TANK_H

#include <string>
#include "../../Utils.h"
#include "../../str/Grafo.h"
#include "../../str/Map.h"

using namespace std;

enum class Color { AZUL, CELESTE, ROJO, AMARILLO };
enum class Equipo { JUGADOR1, JUGADOR2 };

inline string colorATexto(Color color) {
    switch(color) {
        case Color::AZUL:     return "Azul";
        case Color::CELESTE:  return "Celeste";
        case Color::ROJO:     return "Rojo";
        case Color::AMARILLO: return "Amarillo";
        default:              return "Desconocido";
    }
}

class Tank {
    int x;
    int y;
    int vida;
    const Equipo equipo;
    const Color color;

public:
    Tank(int x, int y, int vida, Equipo equipo, Color color);

    virtual void moverse(int nx, int ny, Graph& grafo, Map& map ) = 0;
    void disparar();

    int getX() const;
    int getY() const;
    int getVida() const;
    Color getColor() const;
    Equipo getEquipo() const;
    bool estaVivo() const;
    void recibirDano(int danoRecibido);
    void setPosition(int newX, int newY);
    bool puedeMoverse(int nx, int ny, const Graph& grafo);
    void movimientoAleatorio(int destinoX, int destinoY, Graph& grafo);

    virtual ~Tank() = default;

protected:
    void moverPorCeldas(const Camino& camino, Graph& grafo);
    Punto buscarPosAleatoria(int radio, const Graph& grafo);
    void avanzarHastaDestino(int destinoX, int destinoY, Graph& grafo);
    Camino reconstruirCamino(Graph& grafo, int* padre, int nodoDestino);
};

#endif