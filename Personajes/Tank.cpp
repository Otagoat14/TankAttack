#include <iostream>
#include <vector>
#include <cstdlib>
#include "Tank.h"
#include "../Utils.h"

using namespace std;

Tank::Tank(int x, int y, int vida, Equipo equipo, Color color) : x(x), y(y), vida(vida), equipo(equipo), color(color) {}

void Tank::disparar() {
    // Por implementar
}

int Tank::getX() const { return x; }
int Tank::getY() const { return y; }
int Tank::getVida() const { return vida; }
Color Tank::getColor() const { return color; }
Equipo Tank::getEquipo() const { return equipo; }
bool Tank::estaVivo() const { return vida > 0; }

void Tank::recibirDano(int danoRecibido) {
    vida -= danoRecibido;
    cout << "Recibido " << danoRecibido << endl;
}

void Tank::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

bool Tank::puedeMoverse(int nx, int ny, const Graph& grafo) {
    return grafo.esPasable(ny, nx);
}

void Tank::moverPorCeldas(const vector<pair<int,int>>& celdas, Graph& grafo) {
    for (int i = 1; i < (int)celdas.size(); i++) {
        setPosition(celdas[i].first, celdas[i].second);
    }
}

pair<int,int> Tank::buscarPosAleatoria(int radio, const Graph& grafo) {
    vector<pair<int,int>> candidatos;

    for (int dy = -radio; dy <= radio; dy++) {
        for (int dx = -radio; dx <= radio; dx++) {
            if (dx == 0 && dy == 0) continue;

            int cx = getX() + dx;
            int cy = getY() + dy;

            if (grafo.esPasable(cy, cx)) {
                candidatos.push_back({cx, cy});
            }
        }
    }

    if (candidatos.empty()) {
        cout << "No hay celdas disponibles en el radio" << endl;
        return {-1, -1};
    }

    return candidatos[rand() % candidatos.size()];
}

void Tank::avanzarHastaDestino(int destinoX, int destinoY, Graph& grafo) {
    vector<pair<int,int>> celdas = celdаsLineaVista(getX(), getY(), destinoX, destinoY, grafo);

    if (celdas.size() <= 1) {
        cout << "El tanque no pudo avanzar" << endl;
        return;
    }

    pair<int,int> ultima = celdas.back();
    if (ultima.first == destinoX && ultima.second == destinoY) {
        cout << "Llego al destino" << endl;
    } else {
        cout << "Avanzando hasta donde sea posible" << endl;
    }

    moverPorCeldas(celdas, grafo);
}

void Tank::movimientoAleatorio(int destinoX, int destinoY, Graph& grafo) {
    if (lineaVista(getX(), getY(), destinoX, destinoY, grafo)) {
        cout << "Linea vista libre al destino" << endl;
        avanzarHastaDestino(destinoX, destinoY, grafo);
        return;
    }

    pair<int,int> P = buscarPosAleatoria(2, grafo);
    if (P.first == -1) return;

    avanzarHastaDestino(P.first, P.second, grafo);
    cout << "Segundo intento hacia el destino..." << endl;
    avanzarHastaDestino(destinoX, destinoY, grafo);
}