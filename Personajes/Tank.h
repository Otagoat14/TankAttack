#ifndef TANK_H
#define TANK_H

#include <string>
#include <vector>
#include "../Utils.h"


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
//Cambiar el vector por el grafo
    virtual void moverse(vector<vector<int>>& matriz);

    void disparar();
    int getX() const;
    int getY() const;
    int getVida() const;
    Color getColor() const;
    Equipo getEquipo() const;
    bool estaVivo() const;
    void recibirDano(int danoRecibido);
    void setPosition(int newX, int newY);
    //Igual aqui, cambiar el vector por el grafo
    bool puedeMoverse(int nx, int ny, const vector<vector<int>>& matriz);
    void movimientoAleatorio(int destinoX, int destinoY, vector<vector<int>>& matriz);

    virtual ~Tank() = default;


private:
    //No olvidar cambiar el vector por el grafo luego
    void moverPorCeldas(const vector<pair<int,int>>& celdas, vector<vector<int>>& matriz);
    pair<int,int> buscarPosAleatoria(int radio, const vector<vector<int>>& matriz);
    void avanzarHastaDestino(int destinoX, int destinoY, vector<vector<int>>& matriz);

};

#endif
