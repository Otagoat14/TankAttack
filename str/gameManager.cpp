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

//Crea un power up de forma aleatoria
void gameManager::generarPowerUpAleatorio(int jugador) {
    int r = rand() % 4;
    TipoPowerUp tipo;
    switch (r) {
        case 0: tipo = TipoPowerUp::DOBLE_TURNO;          break;
        case 1: tipo = TipoPowerUp::PRECISION_MOVIMIENTO; break;
        case 2: tipo = TipoPowerUp::PRECISION_ATAQUE;     break;
        default: tipo = TipoPowerUp::PODER_ATAQUE;        break;
    }
    //Asigana el power up al jugador al que le toca en ese turno
    if (jugador == 0) colaJ1.enqueue(tipo);
    else              colaJ2.enqueue(tipo);

    cout << "Power-up generado para jugador " << jugador + 1 << endl;
}

//Establece que un jugador ya realizo una accion
void gameManager::registrarAccion() {
    Action = true;
}

//Retorna si un jugador aun no a realizado ninguna accion
bool gameManager::puedeActuar(int jugador) {
    return jugador == jugadorActivo && !Action;
}

//Obtiene de quien es el turno activo
int gameManager::getJugadorActivo() {
    return jugadorActivo;
}

//Obtiene el tiempk restante
float gameManager::getTiempoRestante() {
    return tiempoRestante;
}

//Retorna si aun el juego esta activo
bool gameManager::juegoAcabado() {
    return !juego;
}

//Obtiene el ganador
int gameManager::getGanador() {
    return ganador;
}

// Retorna si el jugador tiene un power up
bool gameManager::tienePowerUp(int jugador) const {
    if (jugador == 0) return !colaJ1.empty();
    return !colaJ2.empty();
}

//Permite ver el primer power up sin consumirlo
TipoPowerUp gameManager::verSiguientePowerUp(int jugador) const {
    if (jugador == 0) {
        return colaJ1.getFrente();
    }
    return colaJ2.getFrente();
}

//Retorna la cantidad de power ups
int gameManager::getCantidadPowerUps(int jugador) const {
    if (jugador == 0) return colaJ1.getTamano();
    return colaJ2.getTamano();
}

//Aplica un power up retorna false si falla en el proceso
//Accion del turno ya realizada o no posee power ups
bool gameManager::aplicarPowerUp(int jugador) {
    //Verificacion de turno
    if (!puedeActuar(jugador)) return false;

    //Obtiene si tiene poer ups
    Cola<TipoPowerUp>& cola = (jugador == 0) ? colaJ1 : colaJ2;
    if (cola.empty()) return false;

    //Obtiene el tipo de power up
    TipoPowerUp tipo = cola.dequeue();

    //Establece el efecto en base a el tipo de power up
    switch (tipo) {
    case TipoPowerUp::DOBLE_TURNO:
        aplicarDobleTurno();
        break;
    case TipoPowerUp::PRECISION_MOVIMIENTO:
        aplicarPrecisionMovimiento(jugador);
        break;
    case TipoPowerUp::PRECISION_ATAQUE:
        aplicarPrecisionAtaque(jugador);
        break;
    case TipoPowerUp::PODER_ATAQUE:
        aplicarPoderAtaque(jugador);
        break;
    }
    registrarAccion();
    siguienteTurno();
    return true;
}

void gameManager::aplicarDobleTurno() {
    turnosExtrasRestantes = 2;
}

// Activa el efecto de precisión de movimiento para el jugador
void gameManager::aplicarPrecisionMovimiento(int jugador) {
    precisionMovJ[jugador] = true;
}

// Activa el efecto de precisión de ataque para el jugador
void gameManager::aplicarPrecisionAtaque(int jugador) {
    precisionAtaqueJ[jugador] = true;
}

// Activa el efecto de poder de ataque para el jugador
void gameManager::aplicarPoderAtaque(int jugador) {
    poderAtaqueJ[jugador] = true;
}

//Se llama para cada frame
void gameManager::actualizar(float dt) {
    if (!juego) return;

    //Descuenta el tiempo trancurrido y
    //verifica si ya se acabaron los 5 minutos de juego
    //Verifica que jugador tiene mas tanques
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
        //Retorna el jugador ganador
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
    //Retorna el ganador y termina el juego
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
    //Verifica si hay turnos extra
    if (turnosExtrasRestantes > 0) {
        turnosExtrasRestantes--;
        return;
    }

    // Limpiar efectos no usados del jugador que termina su turno
    precisionMovJ[jugadorActivo]    = false;
    precisionAtaqueJ[jugadorActivo] = false;
    poderAtaqueJ[jugadorActivo]     = false;

    //Cambia el jugador que esta activo
    jugadorActivo = (jugadorActivo == 0) ? 1 : 0;
}