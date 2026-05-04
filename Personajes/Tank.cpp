#include <iostream>
#include <vector>
#include "Tank.h"

using namespace std;


Tank::Tank(int x, int y, int vida, Equipo equipo, Color color)
: x(x), y(y), vida(vida), equipo(equipo), color(color) {}

void Tank::moverse(vector<vector<bool>>& matriz) {
    cout << "Moviendose" << endl;
}

void Tank::disparar() {
    cout << "Disparar" << endl;
}
int Tank::getX() const {
    return x;
}
int Tank::getY() const {
    return y;
}
int Tank::getVida() const {
    return vida;
}
Color Tank::getColor() const {
    return color;
}
Equipo Tank::getEquipo() const {
    return equipo;
}
bool Tank::estaVivo() const {
    return vida > 0;
}

void Tank::recibirDano(int danoRecibido) {
    vida -= danoRecibido;
    cout << "Recibido " << danoRecibido << endl;
}

void Tank::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

bool Tank::puedeMoverse(int nx, int ny, const vector<vector<bool>>& matriz) {
    int filas = matriz.size();
    int columnas = matriz[0].size();

    if (nx >= 0 && ny >= 0 && nx < columnas && ny < filas) {
        return matriz[ny][nx] == true;
    }
    return false;
}

