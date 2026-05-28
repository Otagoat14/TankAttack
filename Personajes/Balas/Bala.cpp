#include "Bala.h"
#include "../Tanques/Tank.h"
#include <iostream>
#include <cmath>
#include "../../Utils.h"

using namespace std;

// Constructor
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
      tiempoMovimiento(0.05f),  // La bala se mueve cada 0.05s
      modoAEstrella(modoAEstrella),
      modoPoder(modoPoder) {

    // Calcular la componente de direccion en cada eje
    int dx = destinoX - origenX;
    int dy = destinoY - origenY;
    dirX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    dirY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);
}

//A*
void Bala::avanzarAEstrella(Graph& grafo) {
    // Si ya llego al destino, desactivarse
    if (x == destinoX && y == destinoY) {
        activa = false;
        return;
    }

    // Recalcular el camino desde la posicion actual al destino
    Posicion inicio  = {y, x};
    Posicion destino = {destinoY, destinoX};
    Camino camino    = aStar(grafo, inicio, destino);

    if (camino.empty()) { activa = false; return; } // No hay camino

    // Avanzar solo un paso
    NodoCamino* nodo = camino.getCabeza();
    if (nodo != nullptr) nodo = nodo->siguiente;

    if (nodo == nullptr) { activa = false; return; } // Ya esta en el destino

    x = nodo->x;
    y = nodo->y;
}

// acumula dt y cuando supera tiempoMovimiento avanza un paso.
// Despues de moverse verifica si impacto algun tanque.
void Bala::actualizar(float dt, Graph& grafo, Tank** tanques, int numTanques) {
    if (!activa) return;

    tiempoAcumulado += dt;
    if (tiempoAcumulado < tiempoMovimiento) return; // Aun no es momento de moverse
    tiempoAcumulado = 0;

    // Avanzar segun el modo de disparo
    if (modoAEstrella) {
        avanzarAEstrella(grafo);
    } else {
        avanzarUnPaso(grafo);
    }

    // Solo verificar dano si la bala sigue activa despues de moverse
    if (activa)
        verificarYAplicarDano(tanques, numTanques);
}


// Si la proxima celda esta bloqueada, aplica el rebote correspondiente
// e intenta avanzar con la nueva direccion. Si no quedan rebotes o la
// nueva posicion tambien esta bloqueada, la bala se desactiva
void Bala::avanzarUnPaso(Graph& grafo) {
    int nx = x + dirX; // Proxima columna
    int ny = y + dirY; // Proxima fila

    TipoRebote rebote = detectarRebote(nx, ny, grafo);

    if (rebote != TipoRebote::NINGUNO) {
        // Hay colision
        if (rebotesRestantes <= 0) {
            activa = false; // Sin rebotes restantes, la bala se destruye
            return;
        }
        aplicarRebote(rebote);  // Invertir la direccion segun el tipo de rebote
        rebotesRestantes--;

        // Recalcular la proxima posicion con la nueva direccion
        nx = x + dirX;
        ny = y + dirY;

        // Si la nueva posicion tampoco es valida, desactivar
        if (!grafo.esPasable(ny, nx)) {
            activa = false;
            return;
        }
    }

    // Avanzar a la nueva posicion
    x = nx;
    y = ny;
}

// Compara la posicion actual de la bala con la de cada tanque
// Si hay coincidencia, aplica el dano y desactiva la bala
// Solo impacta al primero que encuentre
void Bala::verificarYAplicarDano(Tank** tanques, int numTanques) {
    for (int i = 0; i < numTanques; i++) {
        if (tanques[i] != nullptr &&
            tanques[i]->getX() == x &&
            tanques[i]->getY() == y) {
            aplicarDano(tanques[i]);
            return; // Impacto encontrado
        }
    }
}


// Se usa vidaMax (no vida actual) para que el dano sea consistente
// sin importar cuanta vida le quede al tanque.
void Bala::aplicarDano(Tank* tanque) {
    if (tanque == nullptr) return;

    int dano;
    if (modoPoder) {
        dano = tanque->getVidaMax(); // Dano total garantizado
    } else {
        Color colorTanque = tanque->getColor();
        int   vidaMax     = tanque->getVidaMax();
        if (colorTanque == Color::CELESTE || colorTanque == Color::AZUL)
            dano = (int)(vidaMax * 0.25f); // Tanques azules son mas resistentes
        else
            dano = (int)(vidaMax * 0.50f); // Tanques rojos/amarillos reciben mas dano
    }

    tanque->recibirDano(dano);
    activa = false; // La bala se destruye al impactar
}


// Verifica si la proxima posicion (nx, ny) esta bloqueada en cada eje.
// Un nodo esta bloqueado si esta fuera de los limites del mapa o es un obstaculo.
TipoRebote Bala::detectarRebote(int nx, int ny, Graph& grafo) {
    int filas = grafo.getRows();
    int cols  = grafo.getCols();

    // Verificar si la proxima columna (nx) esta bloqueada
    bool bloqueadoX = (nx < 0 || nx >= cols || !grafo.esPasable(y, nx));
    // Verificar si la proxima fila (ny) esta bloqueada
    bool bloqueadoY = (ny < 0 || ny >= filas || !grafo.esPasable(ny, x));

    if (bloqueadoX && bloqueadoY) return TipoRebote::ESQUINA;
    if (bloqueadoX)               return TipoRebote::VERTICAL;
    if (bloqueadoY)               return TipoRebote::HORIZONTAL;
    return TipoRebote::NINGUNO;
}


// Invierte las componentes de direccion segun el tipo de rebote.
void Bala::aplicarRebote(TipoRebote tipo) {
    switch (tipo) {
        case TipoRebote::VERTICAL:   dirX = -dirX; break;
        case TipoRebote::HORIZONTAL: dirY = -dirY; break;
        case TipoRebote::ESQUINA:    dirX = -dirX; dirY = -dirY; break;
        default: break;
    }
}


// Getters
bool Bala::estaActiva() const { return activa; }
int  Bala::getX()       const { return x; }
int  Bala::getY()       const { return y; }