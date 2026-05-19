#ifndef TANK_H
#define TANK_H

#include <string>
#include "../../Utils.h"
#include "../../str/Grafo.h"
#include "../../str/Map.h"

using namespace std;

enum class Color { AZUL, CELESTE, ROJO, AMARILLO };
enum class Equipo { JUGADOR1, JUGADOR2 };
enum class Direccion { NORTE, SUR, ESTE, OESTE };

inline string colorATexto(Color color) {
    switch(color) {
        case Color::AZUL:     return "Azul";
        case Color::CELESTE:  return "Celeste";
        case Color::ROJO:     return "Rojo";
        case Color::AMARILLO: return "Amarillo";
        default:              return "Desconocido";
    }
}

class Tank {
    int x;
    int y;
    int vida;
    const Equipo equipo;
    const Color color;
    Direccion direccion;
    NodoCamino* nodoActual;
    Camino* rutaPendiente;
    float tiempoAcumulado;   //Almacena el tiempo transcurrido desde el ultimo movimiento
    float tiempoMovimiento;  // segundos entre cada paso
    bool enMovimiento;

public:
    Tank(int x, int y, int vida, Equipo equipo, Color color);

    virtual void moverse(int nx, int ny, Graph& grafo, Map& map ) = 0;
    void disparar(int destinoX, int destinoY, Graph& grafo, Tank** tanques, int numTanques);

    int getX() const;
    int getY() const;
    int getVida() const;
    Color getColor() const;
    Equipo getEquipo() const;
    Direccion getDireccion();
    bool estaVivo() const;
    void recibirDano(int danoRecibido);
    void setPosition(int newX, int newY);
    void setDireccion(Direccion dir);
    bool puedeMoverse(int nx, int ny, const Graph& grafo);
    void movimientoAleatorio(int destinoX, int destinoY, Graph& grafo);
    void iniciarMovimiento(Camino* camino);  // recibe la ruta
    void actualizar(float dt);               // avanza un paso si pasó el tiempo, dt = delta time de main
    bool estaEnMovimiento() const;

    ~Tank();

protected:
    void moverPorCeldas(const Camino& camino, Graph& grafo);
    Punto buscarPosAleatoria(int radio, const Graph& grafo);
    void avanzarHastaDestino(int destinoX, int destinoY, Graph& grafo);
    Camino reconstruirCamino(Graph& grafo, int* padre, int nodoDestino);
};

#endif