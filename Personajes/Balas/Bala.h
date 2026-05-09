
#ifndef BALA_H
#define BALA_H

#include "../../Utils.h"
#include "../../str/Grafo.h"
#include "Tank.h"

//Para modificar mas facil la cantidad de rebotes
const int MAX_REBOTES = 4;

enum class TipoRebote {
    VERTICAL,    // pared al costado, invierte dirX
    HORIZONTAL,  // pared arriba/abajo, invierte dirY
    ESQUINA,     // ambas, invierte dirX y dirY
    NINGUNO      // no hay rebote
};

class Bala {
    int x;
    int y;
    int dirX;
    int dirY;
    int destinoX;
    int destinoY;
    int rebotesRestantes;
    bool activa;
    Tank* tanqueOrigen;

public:
    Bala(int origenX, int origenY,
         int destinoX, int destinoY,
         Tank* tanqueOrigen);

    //Mueve la bala un paso, maneja rebote si es necesario
    void mover(Graph& grafo);

    //Verifica si la bala tocó algún tanque de la lista
    // Retorna el tanque golpeado o nullptr si no tocó ninguno
    Tank* verificarColision(Tank** tanques, int numTanques);

    //Aplica dano según el color del tanque golpeado
    void aplicarDano(Tank* tanque);

    bool estaActiva() const;
    int getX() const;
    int getY() const;

private:
    // Detecta qué tipo de pared golpeó la bala
    TipoRebote detectarRebote(int nx, int ny, Graph& grafo);

    // Invierte la dirección según el tipo de rebote
    void aplicarRebote(TipoRebote tipo);
};

#endif