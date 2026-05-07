#include <iostream>
#include <vector>
#include <cstdlib>
#include "Tank.h"
#include "../Utils.h"

using namespace std;


Tank::Tank(int x, int y, int vida, Equipo equipo, Color color)
: x(x), y(y), vida(vida), equipo(equipo), color(color) {}

void Tank::moverse(vector<vector<int>>& matriz) {
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

bool Tank::puedeMoverse(int nx, int ny, const Graph& grafo) {
    int filas = matriz.size();
    int columnas = matriz[0].size();

    if (nx >= 0 && ny >= 0 && nx < columnas && ny < filas) {
        return matriz[ny][nx] == true;
    }
    return false;
}

//Cambiar el vector
void Tank::moverPorCeldas(const vector<pair<int,int>>& celdas, Graph& grafo) {
    for (int i = 1; i < celdas.size(); i++) {
        matriz[getY()][getX()] = 0;
        setPosition(celdas[i].first, celdas[i].second);
    }
}

//Cambair el vector
pair<int,int> Tank::buscarPosAleatoria(int radio, const Graph& grafo) {
    int filas    = matriz.size();
    int columnas = matriz[0].size();

    vector<pair<int,int>> candidatos;

    for (int dy = -radio; dy <= radio; dy++) {
        for (int dx = -radio; dx <= radio; dx++) {
            if (dx == 0 && dy == 0) continue;

            int cx = getX() + dx;
            int cy = getY() + dy;

            if (cx >= 0 && cy >= 0 && cx < columnas && cy < filas
                && matriz[cy][cx]) {
                candidatos.push_back({cx, cy});
            }
        }
    }

    if (candidatos.empty()) {
        cout << "No hay celdas disponibles en el radio" << endl;
        return {-1, -1};
    }

    int indice = rand() % candidatos.size();
    return candidatos[indice];
}

void Tank::avanzarHastaDestino(int destinoX, int destinoY, Graph& grafo) {

    vector<pair<int,int>> celdas = celdаsLineaVista(getX(), getY(),destinoX, destinoY, matriz);
    if (celdas.size() <= 1) {
        cout << "El tanque no pudo avanzar" << endl;
        return;
    }

    // Si la última celda es el destino, llegó completo
    pair<int,int> ultima = celdas.back();
    if (ultima.first == destinoX && ultima.second == destinoY) {
        cout << "Llegó al destino" << endl;
    } else {
        cout << "Avanzando hasta donde sea posible" << endl;
    }

    moverPorCeldas(celdas, matriz);
}

void Tank::movimientoAleatorio(int destinoX, int destinoY, Graph& grafo) {

    if (lineaVista(getX(), getY(), destinoX, destinoY, matriz)) {
        cout << "Linea vista libre al destino" << endl;
        avanzarHastaDestino(destinoX, destinoY, matriz);
        return;
    }
    pair<int,int> P = buscarPosAleatoria(2, matriz);

    if (P.first == -1) {
        return;
    }
    avanzarHastaDestino(P.first, P.second, matriz);
    cout << "Segundo intento hacia el destino..." << endl;
    avanzarHastaDestino(destinoX, destinoY, matriz);
}
