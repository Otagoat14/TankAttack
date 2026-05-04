#ifndef TANK_H
#define TANK_H

using namespace std;
enum class Color { AZUL, CELESTE, ROJO, AMARILLO };
enum class Equipo { JUGADOR1, JUGADOR2 };

//Esta funcion esta para cuando toque hacer validaciones de colores
inline string colorATexto(Color color) {
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
    Tank(int x, int y, int vida, Equipo equipo, Color color);

    virtual void moverse(vector<vector<bool>>& matriz);

    void disparar();
    int getX() const;
    int getY() const;
    int getVida() const;
    Color getColor() const;
    Equipo getEquipo() const;
    bool estaVivo() const;
    void recibirDano(int danoRecibido);
    void setPosition(int newX, int newY);
    bool puedeMoverse(int nx, int ny, const vector<vector<bool>>& matriz);
    virtual ~Tank() = default;

};

#endif
