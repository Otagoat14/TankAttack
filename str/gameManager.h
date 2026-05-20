//
// Created by nacho on 5/19/2026.
//

#ifndef TANKATTACK_GAMEMANAGER_H
#define TANKATTACK_GAMEMANAGER_H
#include "../Personajes/Tanques/Tank.h"


class gameManager
{
    private:
    Tank** tank;            //Puntero a los tanques pra verficaciones
    int jugadorActivo;      //Jugador activo
    bool Action;            //(Variable de control)El jugador ya utilizo la accion de ese turno
    float tiempoRestante;
    bool juego;             //El juego esta activo o no
    int ganador;            // -1 sin ganador definido

    public:

    gameManager(Tank** tanques);
    ~gameManager();
    void siguienteTurno();          //Pasa de turno
    void registrarAccion();         //Guarda si un jugador ya realizo una accion
    int getJugadorActivo();         //Obtiene de quien es el turno
    bool puedeActuar(int jugador);             //Comprueba que el jugador no alla hecho ninguna accion antes
    void actualizar(float dt);              //Actualiza el tiempo restnate desopues de cada frame
    float getTiempoRestante();      //Retorna el tiempo que queda
    bool juegoAcabado();            //Retorna si el juego termino
    int getGanador();               //Obtiene al ganador


};


#endif //TANKATTACK_GAMEMANAGER_H