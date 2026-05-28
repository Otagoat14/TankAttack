#ifndef BALA_H
#define BALA_H

#include "../../Utils.h"
#include "../../str/Grafo.h"

const int MAX_REBOTES = 8;

enum class TipoRebote {
    VERTICAL,
    HORIZONTAL,
    ESQUINA,
    NINGUNO
};

// Declaración adelantada para evitar include circular
class Tank;

class Bala {
    int x;
    int y;
    int dirX;
    int dirY;
    int destinoX;
    int destinoY;
    int rebotesRestantes;
    bool activa;
    bool modoAEstrella;
    bool modoPoder;
    Tank* tanqueOrigen;

    //Animación
    float tiempoAcumulado;
    float tiempoMovimiento;  // segundos entre cada paso

public:
    Bala(int origenX, int origenY,
         int destinoX, int destinoY,
         Tank* tanqueOrigen,
         bool modoAEstrella = false,
         bool modoPoder = false);

    // Ahora recibe dt en lugar de moverse instantáneamente
    void actualizar(float dt, Graph& grafo, Tank** tanques, int numTanques);

    bool estaActiva() const;
    int getX() const;
    int getY() const;
    bool esModoAEstrella() const { return modoAEstrella; }
    bool esModoPoder()     const { return modoPoder; }
    void avanzarAEstrella(Graph& grafo);

private:
    TipoRebote detectarRebote(int nx, int ny, Graph& grafo);
    void aplicarRebote(TipoRebote tipo);
    void avanzarUnPaso(Graph& grafo);
    void verificarYAplicarDano(Tank** tanques, int numTanques);
    void aplicarDano(Tank* tanque);
};

#endif