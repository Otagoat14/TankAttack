#include "gameManager.h"
#include <iostream>
#include <cstdlib>
using namespace std;

gameManager::gameManager(Tank** tanques) {
    this->tank       = tanques;
    jugadorActivo    = 0;
    Action           = false;
    tiempoRestante   = 300.f;
    juego            = true;
    ganador          = -1;
    turnosExtrasRestantes = 0;

    for (int i = 0; i < 2; i++) {
        tiempoProxPowerUp[i]  = 15.f + rand() % 20;  // primero entre 15-35s
        precisionMovJ[i]      = false;
        precisionAtaqueJ[i]   = false;
        poderAtaqueJ[i]       = false;
    }
}

gameManager::~gameManager() {}

void gameManager::generarPowerUpAleatorio(int jugador) {
    int r = rand() % 4;
    TipoPowerUp tipo;
    switch (r) {
        case 0: tipo = TipoPowerUp::DOBLE_TURNO;          break;
        case 1: tipo = TipoPowerUp::PRECISION_MOVIMIENTO; break;
        case 2: tipo = TipoPowerUp::PRECISION_ATAQUE;     break;
        default: tipo = TipoPowerUp::PODER_ATAQUE;        break;
    }
    if (jugador == 0) colaJ1.enqueue(tipo);
    else              colaJ2.enqueue(tipo);

    cout << "Power-up generado para jugador " << jugador + 1 << endl;
}

void gameManager::registrarAccion() {
    Action = true;
}

bool gameManager::puedeActuar(int jugador) {
    return jugador == jugadorActivo && !Action;
}

int gameManager::getJugadorActivo() {
    return jugadorActivo;
}

float gameManager::getTiempoRestante() {
    return tiempoRestante;
}

bool gameManager::juegoAcabado() {
    return !juego;
}

int gameManager::getGanador() {
    return ganador;
}

bool gameManager::tienePowerUp(int jugador) const {
    if (jugador == 0) return !colaJ1.empty();
    return !colaJ2.empty();
}

TipoPowerUp gameManager::verSiguientePowerUp(int jugador) const {
    if (jugador == 0) {
        return colaJ1.getFrente();
    }
    return colaJ2.getFrente();
}

int gameManager::getCantidadPowerUps(int jugador) const {
    if (jugador == 0) return colaJ1.getTamano();
    return colaJ2.getTamano();
}

bool gameManager::aplicarPowerUp(int jugador) {
    if (!puedeActuar(jugador)) return false;

    Cola<TipoPowerUp>& cola = (jugador == 0) ? colaJ1 : colaJ2;
    if (cola.empty()) return false;

    TipoPowerUp tipo = cola.dequeue();

    switch (tipo) {
        case TipoPowerUp::DOBLE_TURNO:
            turnosExtrasRestantes = 2;
            cout << "Power-up: DOBLE TURNO activado" << endl;
            break;
        case TipoPowerUp::PRECISION_MOVIMIENTO:
            precisionMovJ[jugador] = true;
            cout << "Power-up: PRECISION MOVIMIENTO activado" << endl;
            break;
        case TipoPowerUp::PRECISION_ATAQUE:
            precisionAtaqueJ[jugador] = true;
            cout << "Power-up: PRECISION ATAQUE activado" << endl;
            break;
        case TipoPowerUp::PODER_ATAQUE:
            poderAtaqueJ[jugador] = true;
            cout << "Power-up: PODER ATAQUE activado" << endl;
            break;
    }

    // Consumir el turno
    registrarAccion();
    siguienteTurno();
    return true;
}

void gameManager::actualizar(float dt) {
    if (!juego) return;

    tiempoRestante -= dt;
    if (tiempoRestante <= 0) {
        tiempoRestante = 0;
        int vivosJ1 = 0, vivosJ2 = 0;
        for (int i = 0; i < 4; i++) {
            if (tank[i]->estaVivo()) {
                if (tank[i]->getEquipo() == Equipo::JUGADOR1) vivosJ1++;
                else vivosJ2++;
            }
        }
        ganador = (vivosJ1 >= vivosJ2) ? 0 : 1;
        juego   = false;
        return;
    }

    // Verificar si algún jugador perdió todos sus tanques
    int vivosJ1 = 0, vivosJ2 = 0;
    for (int i = 0; i < 4; i++) {
        if (tank[i]->estaVivo()) {
            if (tank[i]->getEquipo() == Equipo::JUGADOR1) vivosJ1++;
            else vivosJ2++;
        }
    }
    if (vivosJ1 == 0) { ganador = 1; juego = false; return; }
    if (vivosJ2 == 0) { ganador = 0; juego = false; return; }

    // Generar power-ups aleatoriamente
    for (int j = 0; j < 2; j++) {
        tiempoProxPowerUp[j] -= dt;
        if (tiempoProxPowerUp[j] <= 0) {
            generarPowerUpAleatorio(j);
            tiempoProxPowerUp[j] = 20.f + rand() % 25;  // próximo entre 20-45s
        }
    }
}

void gameManager::siguienteTurno() {
    Action = false;

    if (turnosExtrasRestantes > 0) {
        turnosExtrasRestantes--;
        // El jugador activo se queda igual
        return;
    }

    jugadorActivo = (jugadorActivo == 0) ? 1 : 0;
}