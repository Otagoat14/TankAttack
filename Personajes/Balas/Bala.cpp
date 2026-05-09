#include "Bala.h"
#include "../Tanques/Tank.h"
#include <iostream>
#include <cmath>

class Tank;
using namespace std;

Bala::Bala(int origenX, int origenY, int destinoX, int destinoY, Tank* tanqueOrigen) : x(origenX), y(origenY),
      destinoX(destinoX), destinoY(destinoY),
      rebotesRestantes(MAX_REBOTES),
      activa(true),
      tanqueOrigen(tanqueOrigen) {

    // Calcular dirección inicial hacia el destino
    // Normalizar a -1, 0 o 1
    int dx = destinoX - origenX;
    int dy = destinoY - origenY;

    dirX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    dirY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);
}

TipoRebote Bala::detectarRebote(int nx, int ny, Graph& grafo) {
    int filas = grafo.getTotalNodos() / grafo.getCols();
    int cols  = grafo.getCols();

    // Verificar si la siguiente celda en X está bloqueada
    bool bloqueadoX = (nx < 0 || nx >= cols ||
                       !grafo.esPasable(y, nx));

    // Verificar si la siguiente celda en Y está bloqueada
    bool bloqueadoY = (ny < 0 || ny >= filas ||
                       !grafo.esPasable(ny, x));

    if (bloqueadoX && bloqueadoY) return TipoRebote::ESQUINA;
    if (bloqueadoX)               return TipoRebote::VERTICAL;
    if (bloqueadoY)               return TipoRebote::HORIZONTAL;
    return TipoRebote::NINGUNO;
}

void Bala::aplicarRebote(TipoRebote tipo) {
    switch (tipo) {
        case TipoRebote::VERTICAL:
            dirX = -dirX;
            break;
        case TipoRebote::HORIZONTAL:
            dirY = -dirY;
            break;
        case TipoRebote::ESQUINA:
            dirX = -dirX;
            dirY = -dirY;
            break;
        case TipoRebote::NINGUNO:
            break;
    }
}

void Bala::mover(Graph& grafo) {
    if (!activa) return;

    int nx = x + dirX;
    int ny = y + dirY;

    TipoRebote rebote = detectarRebote(nx, ny, grafo);

    if (rebote != TipoRebote::NINGUNO) {
        if (rebotesRestantes <= 0) {
            // Sin rebotes disponibles, la bala se detiene
            activa = false;
            cout << "Bala detenida en: (" << x << "," << y << ")" << endl;
            return;
        }

        aplicarRebote(rebote);
        rebotesRestantes--;
        cout << "Rebote, Quedan: " << rebotesRestantes << endl;

        // Recalcular siguiente posición con nueva dirección
        nx = x + dirX;
        ny = y + dirY;

        // Si sigue bloqueado tras el rebote, la bala se detiene
        if (!grafo.esPasable(ny, nx)) {
            activa = false;
            return;
        }
    }

    x = nx;
    y = ny;
    cout << "Bala en: (" << x << "," << y << ")" << endl;
}

Tank* Bala::verificarColision(Tank** tanques, int numTanques) {
    for (int i = 0; i < numTanques; i++) {
        if (tanques[i] != nullptr &&
            tanques[i]->getX() == x &&
            tanques[i]->getY() == y) {
            return tanques[i];
        }
    }
    return nullptr;
}

void Bala::aplicarDano(Tank* tanque) {
    if (tanque == nullptr) return;

    Color colorTanque = tanque->getColor();
    int vidaActual    = tanque->getVida();

    // 25% para celeste/azul, 50% para amarillo/rojo
    int dano;
    if (colorTanque == Color::CELESTE || colorTanque == Color::AZUL) {
        dano = vidaActual * 0.25;
    } else {
        dano = vidaActual * 0.50;
    }

    tanque->recibirDano(dano);
    activa = false;
    cout << "Impacto, Dano: " << dano << endl;
}

bool Bala::estaActiva() const { return activa; }
int  Bala::getX()       const { return x; }
int  Bala::getY()       const { return y; }