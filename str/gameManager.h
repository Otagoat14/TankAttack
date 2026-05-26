#ifndef TANKATTACK_GAMEMANAGER_H
#define TANKATTACK_GAMEMANAGER_H

#include "../Personajes/Tanques/Tank.h"
#include "../Utils.h"

class gameManager {
private:
    Tank** tank;
    int  jugadorActivo;
    bool Action;
    float tiempoRestante;
    bool juego;
    int  ganador;

    // ── Power-ups ──
    Cola<TipoPowerUp> colaJ1;
    Cola<TipoPowerUp> colaJ2;

    float tiempoProxPowerUp[2];   // cuándo se genera el próximo power-up por jugador

    // Estado de efectos activos
    int  turnosExtrasRestantes;   // para DOBLE_TURNO
    bool precisionMovJ[2];        // PRECISION_MOVIMIENTO activa por jugador
    bool precisionAtaqueJ[2];     // PRECISION_ATAQUE activa por jugador
    bool poderAtaqueJ[2];         // PODER_ATAQUE activo por jugador

    void generarPowerUpAleatorio(int jugador);

public:
    gameManager(Tank** tanques);
    ~gameManager();

    void siguienteTurno();
    void registrarAccion();
    int  getJugadorActivo();
    bool puedeActuar(int jugador);
    void actualizar(float dt);
    float getTiempoRestante();
    bool juegoAcabado();
    int  getGanador();

    // Power-ups
    bool aplicarPowerUp(int jugador);       // consume uno de la cola (turno gastado)
    bool tienePowerUp(int jugador) const;
    TipoPowerUp verSiguientePowerUp(int jugador) const;

    // Consultas de efectos activos (para tanques y balas)
    bool getPrecisionMov(int jugador)    const { return precisionMovJ[jugador]; }
    bool getPrecisionAtaque(int jugador) const { return precisionAtaqueJ[jugador]; }
    bool getPoderAtaque(int jugador)     const { return poderAtaqueJ[jugador]; }

    // Consumir efecto tras usarlo
    void consumirPrecisionMov(int jugador)    { precisionMovJ[jugador]    = false; }
    void consumirPrecisionAtaque(int jugador) { precisionAtaqueJ[jugador] = false; }
    void consumirPoderAtaque(int jugador)     { poderAtaqueJ[jugador]     = false; }

    int getCantidadPowerUps(int jugador) const;
};

#endif