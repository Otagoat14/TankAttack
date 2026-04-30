#include <iostream>



using namespace std;
enum class Color {
    AZUL,
    CELESTE,
    ROJO,
    AMARILLO
};

enum class Equipo {
    JUGADOR1,
    JUGADOR2,
};

string colorATexto(Color color) {
    switch(color) {
        case Color::AZUL:
            return "Azul";
        case Color::CELESTE:
            return "Celeste";
        case Color::ROJO:
            return "Rojo";
        case Color::AMARILLO:
            return "Amarillo";
        default:
            return "Desconocido";
    }
}

class Tank {
    int x;
    int y;
    int vida;
    const Equipo equipo;
    const Color color;

    public:

    Tank(int x, int y,  int vida, Equipo equipo, Color color)
    : x(x), y(y), vida(vida), equipo(equipo), color(color) {
    }

    virtual void moverse() {
        cout << "Moviendose" << endl;
    }

    void disparar() {
        cout << "Disparar" << endl;
    }

    void morir() {
        cout << "Morir" << endl;
    }

    virtual ~Tank() = default;
};


class tankBFS : public Tank {
public:
    tankBFS(int x, int y, const Equipo equipo, const Color color, int vida) : Tank(x, y,  vida, equipo, color) {}

    void moverse() override {
    }
};

class tankDijkstra : public Tank {
public:
    tankDijkstra(int x, int y, const Equipo equipo, const Color color, int vida) : Tank(x, y,  vida, equipo, color) {}

    void moverse() override {
    }
};

