#ifndef TANK_H
#define TANK_H

#include <string>
#include "../../Utils.h"
#include "../../str/Grafo.h"
#include "../../str/Map.h"
#include "../Balas/Bala.h"

using namespace std;


//Propiedades de los tanques, obliga a escribir Color::AZUL
//en lugar de solo AZUL, lo que evita confusiones entre enums distintos.
enum class Color { AZUL, CELESTE, ROJO, AMARILLO };
enum class Equipo { JUGADOR1, JUGADOR2 };
enum class Direccion { NORTE, SUR, ESTE, OESTE };

//Convertir el color a texto
//inline le dice al compilador que copie el cuerpo de la función donde se llama,
//en lugar de hacer un salto de función.
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
    //Pocision de los tanques (columna, fila)
    int x, y;
    //Vida actual y la vida maxima, como el dano se calcula con
    //porcentaje, si no se ponia vida max con parametro a commparar
    //ocurria un bug de que recibia cierto porcentaje de dano,
    //segun la vida que tenia, por lo que nunca moria
    int vida, vidaMax;
    const Equipo equipo;
    const Color color;
    Direccion direccion;
    NodoCamino* nodoActual;
    Camino* rutaPendiente;
    //Almacena el tiempo transcurrido desde el ultimo movimiento
    float tiempoAcumulado;
    // segundos entre cada paso
    float tiempoMovimiento;
    bool enMovimiento;

public:
    Tank(int x, int y, int vida, Equipo equipo, Color color);


    //El = 0 lo hace virtual puro, entonces los otros tanues tionen que crear su propia version de moverse
    virtual void moverse(int nx, int ny, Graph& grafo, Map& map, bool altaPrecision = false) = 0;
    // Crea y retorna una bala lista para ser animada
    Bala* disparar(int destinoX, int destinoY, bool modoAEstrella = false, bool modoPoder     = false);

    int getX() const;
    int getY() const;
    int getVida() const;
    int getVidaMax() const;
    Color getColor() const;
    Equipo getEquipo() const;
    Direccion getDireccion();
    bool estaVivo() const;
    void recibirDano(int danoRecibido);
    void setPosition(int newX, int newY);
    void setDireccion(Direccion dir);
    bool puedeMoverse(int nx, int ny, const Graph& grafo);
    void movimientoAleatorio(int destinoX, int destinoY, Graph& grafo);
    // recibe la ruta
    void iniciarMovimiento(Camino* camino);
    // avanza un paso si pasó el tiempo, dt = delta time de main, que es cuanto
    //tiempo  paso entre cada frame
    void actualizar(float dt);
    bool estaEnMovimiento() const;
    //Es una lista enlazada con la ruta calculada para dibujarla
    Camino* getRutaPendiente() const { return rutaPendiente; }

    ~Tank();

protected:
    void moverPorCeldas(const Camino& camino, Graph& grafo);
    Punto buscarPosAleatoria(int radio, const Graph& grafo);
    void avanzarHastaDestino(int destinoX, int destinoY, Graph& grafo);
    Camino reconstruirCamino(Graph& grafo, int* padre, int nodoDestino);
};

#endif