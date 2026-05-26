#include "Bala.h"
#include "../Tanques/Tank.h"
#include <iostream>
#include <cmath>
#include "../../Utils.h"

using namespace std;

Bala::Bala(int origenX, int origenY,
           int destinoX, int destinoY,
           Tank* tanqueOrigen,
           bool modoAEstrella,
           bool modoPoder)
    : x(origenX), y(origenY),
      destinoX(destinoX), destinoY(destinoY),
      rebotesRestantes(MAX_REBOTES),
      activa(true),
      tanqueOrigen(tanqueOrigen),
      tiempoAcumulado(0),
      tiempoMovimiento(0.05f),
      modoAEstrella(modoAEstrella),
      modoPoder(modoPoder) {

    int dx = destinoX - origenX;
    int dy = destinoY - origenY;
    dirX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    dirY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);
}

void Bala::avanzarAEstrella(Graph& grafo) {
    if (x == destinoX && y == destinoY) {
        activa = false;
        return;
    }

    Posicion inicio  = {y, x};
    Posicion destino = {destinoY, destinoX};
    Camino camino    = aStar(grafo, inicio, destino);

    if (camino.empty()) { activa = false; return; }

    // Tomar el segundo nodo (el primero es la posición actual)
    NodoCamino* nodo = camino.getCabeza();
    if (nodo != nullptr) nodo = nodo->siguiente;

    if (nodo == nullptr) { activa = false; return; }

    x = nodo->x;
    y = nodo->y;
}

void Bala::actualizar(float dt, Graph& grafo, Tank** tanques, int numTanques) {
    if (!activa) return;

    tiempoAcumulado += dt;
    if (tiempoAcumulado < tiempoMovimiento) return;
    tiempoAcumulado = 0;

    if (modoAEstrella) {
        avanzarAEstrella(grafo);
    } else {
        avanzarUnPaso(grafo);
    }

    if (activa)
        verificarYAplicarDano(tanques, numTanques);
}

void Bala::avanzarUnPaso(Graph& grafo) {
    int nx = x + dirX;
    int ny = y + dirY;

    TipoRebote rebote = detectarRebote(nx, ny, grafo);

    if (rebote != TipoRebote::NINGUNO) {
        if (rebotesRestantes <= 0) {
            activa = false;
            return;
        }
        aplicarRebote(rebote);
        rebotesRestantes--;

        nx = x + dirX;
        ny = y + dirY;

        if (!grafo.esPasable(ny, nx)) {
            activa = false;
            return;
        }
    }

    x = nx;
    y = ny;
}

void Bala::verificarYAplicarDano(Tank** tanques, int numTanques) {
    for (int i = 0; i < numTanques; i++) {
        if (tanques[i] != nullptr &&
            tanques[i]->getX() == x &&
            tanques[i]->getY() == y) {
            aplicarDano(tanques[i]);
            return;
        }
    }
}

void Bala::aplicarDano(Tank* tanque) {
    if (tanque == nullptr) return;

    int dano;
    if (modoPoder) {
        dano = tanque->getVidaMax();   // 100% de daño
    } else {
        Color colorTanque = tanque->getColor();
        int   vidaMax     = tanque->getVidaMax();
        if (colorTanque == Color::CELESTE || colorTanque == Color::AZUL)
            dano = (int)(vidaMax * 0.25f);
        else
            dano = (int)(vidaMax * 0.50f);
    }

    tanque->recibirDano(dano);
    activa = false;
}

TipoRebote Bala::detectarRebote(int nx, int ny, Graph& grafo) {
    int filas = grafo.getRows();
    int cols  = grafo.getCols();

    bool bloqueadoX = (nx < 0 || nx >= cols || !grafo.esPasable(y, nx));
    bool bloqueadoY = (ny < 0 || ny >= filas || !grafo.esPasable(ny, x));

    if (bloqueadoX && bloqueadoY) return TipoRebote::ESQUINA;
    if (bloqueadoX)               return TipoRebote::VERTICAL;
    if (bloqueadoY)               return TipoRebote::HORIZONTAL;
    return TipoRebote::NINGUNO;
}

void Bala::aplicarRebote(TipoRebote tipo) {
    switch (tipo) {
        case TipoRebote::VERTICAL:   dirX = -dirX; break;
        case TipoRebote::HORIZONTAL: dirY = -dirY; break;
        case TipoRebote::ESQUINA:    dirX = -dirX; dirY = -dirY; break;
        default: break;
    }
}



bool Bala::estaActiva() const { return activa; }
int  Bala::getX()       const { return x; }
int  Bala::getY()       const { return y; }