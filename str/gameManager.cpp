//
// Created by nacho on 5/19/2026.
//

#include "gameManager.h"
#include <iostream>
using namespace std;

gameManager::gameManager(Tank** tanques) {
    this->tank = tanques;
    jugadorActivo = 0;
    Action = false;
    tiempoRestante = 300.f;
    juego = true;
    ganador = -1;
}

gameManager::~gameManager() {}

void gameManager::registrarAccion() {
    Action = true;
}

bool gameManager::puedeActuar(int jugador)
{
    return jugador == jugadorActivo && !Action;
}

int gameManager::getJugadorActivo() {
    return jugadorActivo;
}

float gameManager::getTiempoRestante()
{
    return tiempoRestante;
}

bool gameManager::juegoAcabado() {
    return !juego;
}

int gameManager::getGanador() {
    return ganador;
}

void gameManager::actualizar(float dt)
{
    //Verifica si el juego termino
    if (!juego) return;

    // Descontar tiempo
    tiempoRestante -= dt;
    if (tiempoRestante <= 0) {
        tiempoRestante = 0;
        // Contar tanques vivos por jugador
        int vivosJ1 = 0, vivosJ2 = 0;
        for (int i = 0; i < 4; i++) {
            if (tank[i]->estaVivo()) {
                if (tank[i]->getEquipo() == Equipo::JUGADOR1) vivosJ1++;
                else vivosJ2++;
            }
        }
        if (vivosJ1 > vivosJ2)
        {
            ganador = 0;
        } else { ganador = 1;}
        juego = false;
    }

    // Verificar si algún jugador perdió todos sus tanques
    int vivosJ1 = 0, vivosJ2 = 0;
    for (int i = 0; i < 4; i++) {
        if (tank[i]->estaVivo()) {
            if (tank[i]->getEquipo() == Equipo::JUGADOR1) vivosJ1++;
            else vivosJ2++;
        }
    }
    if (vivosJ1 == 0) { ganador = 1; juego = false; }
    if (vivosJ2 == 0) { ganador = 0; juego = false; }
}

void gameManager::siguienteTurno()
{
    Action = false;
    if (jugadorActivo == 0)
    {
        jugadorActivo = 1;
    } else {jugadorActivo = 0;}
}