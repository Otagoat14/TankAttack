#ifndef TANK_H
#define TANK_H

#include <string>
#include <vector>
#include "../Utils.h"
#include "../str/Grafo.h"

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

    virtual void moverse(Graph& grafo) = 0;
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

private:
    void moverPorCeldas(const vector<pair<int,int>>& celdas, Graph& grafo);
    pair<int,int> buscarPosAleatoria(int radio, const Graph& grafo);
    void avanzarHastaDestino(int destinoX, int destinoY, Graph& grafo);
};

#endif