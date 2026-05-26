#include <iostream>
#include "Tank.h"
#include "../../Utils.h"
#include "../Balas/Bala.h"

using namespace std;

Tank::Tank(int x, int y, int vida, Equipo equipo, Color color) : x(x), y(y), vida(vida), vidaMax(vida), equipo(equipo), color(color)
{
    rutaPendiente = nullptr;
    tiempoAcumulado = 0;
    tiempoMovimiento = 0.15;
    enMovimiento = false;
    nodoActual = nullptr;
}

Bala* Tank::disparar(int destinoX, int destinoY,
                     bool modoAEstrella, bool modoPoder) {
    return new Bala(getX(), getY(), destinoX, destinoY, this,
                    modoAEstrella, modoPoder);
}

int Tank::getX() const { return x; }
int Tank::getY() const { return y; }
int Tank::getVida() const { return vida; }
int Tank::getVidaMax() const { return vidaMax; }
Color Tank::getColor() const { return color; }
Equipo Tank::getEquipo() const { return equipo; }
Direccion Tank::getDireccion() { return direccion; }
bool Tank::estaVivo() const { return vida > 0; }

void Tank::recibirDano(int danoRecibido) {
    vida -= danoRecibido;
    cout << "Recibido " << danoRecibido << endl;
}

void Tank::setPosition(int newX, int newY) {
    if (newX != x)
    {
        if (newX < x)
        {
            direccion = Direccion::ESTE;
        } else { direccion = Direccion::OESTE; }
    } else
    {
        if (newY < y)
        {
            direccion = Direccion::SUR;
        } else { direccion = Direccion::NORTE; }
    }
    x = newX;
    y = newY;
}

void Tank::setDireccion(Direccion dir) { direccion = dir; }

bool Tank::puedeMoverse(int nx, int ny, const Graph& grafo) {
    return grafo.esPasable(ny, nx);
}

void Tank::moverPorCeldas(const Camino& camino, Graph& grafo) {
    NodoCamino* actual = camino.getCabeza();

    // Saltamos el primer nodo ya que es la posición actual del tanque
    if (actual != nullptr)
        actual = actual->siguiente;

    while (actual != nullptr) {
        setPosition(actual->x, actual->y);
        actual = actual->siguiente;
    }
}

Punto Tank::buscarPosAleatoria(int radio, const Graph& grafo) {
    // Primera pasada: contar candidatos válidos
    int count = 0;
    for (int dy = -radio; dy <= radio; dy++) {
        for (int dx = -radio; dx <= radio; dx++) {
            if (dx == 0 && dy == 0) continue;
            if (grafo.esPasable(getY() + dy, getX() + dx))
                count++;
        }
    }

    if (count == 0) return Punto(-1, -1);

    // Segunda pasada: elegir el índice aleatorio
    int elegido = rand() % count;
    int actual = 0;
    for (int dy = -radio; dy <= radio; dy++) {
        for (int dx = -radio; dx <= radio; dx++) {
            if (dx == 0 && dy == 0) continue;
            if (grafo.esPasable(getY() + dy, getX() + dx)) {
                if (actual == elegido)
                    return Punto(getX() + dx, getY() + dy);
                actual++;
            }
        }
    }
    return Punto(-1, -1);
}

void Tank::avanzarHastaDestino(int destinoX, int destinoY, Graph& grafo) {
    Camino camino = celdаsLineaVista(getX(), getY(), destinoX, destinoY, grafo);

    if (camino.getTamano() <= 1) {
        cout << "El tanque no pudo avanzar" << endl;
        return;
    }

    // Revisar si el ultimo nodo es el destino
    NodoCamino* actual = camino.getCabeza();
    while (actual->siguiente != nullptr)
        actual = actual->siguiente;

    if (actual->x == destinoX && actual->y == destinoY) {
        cout << "Llego al destino" << endl;
    } else {
        cout << "Avanzando hasta donde sea posible" << endl;
    }

    moverPorCeldas(camino, grafo);
}

void Tank::movimientoAleatorio(int destinoX, int destinoY, Graph& grafo) {
    if (lineaVista(getX(), getY(), destinoX, destinoY, grafo)) {
        cout << "Linea vista libre al destino" << endl;
        avanzarHastaDestino(destinoX, destinoY, grafo);
        return;
    }

    Punto p = buscarPosAleatoria(2, grafo);
    if (p.x == -1) return;

    avanzarHastaDestino(p.x, p.y, grafo);
    cout << "Segundo intento hacia el destino..." << endl;
    avanzarHastaDestino(destinoX, destinoY, grafo);
}

Camino Tank::reconstruirCamino(Graph& grafo, int* padre, int nodoDestino) {
    // Primera pasada - contar longitud
    int longitud = 0;
    int actual   = nodoDestino;
    while (actual != -1) {
        longitud++;
        actual = padre[actual];
    }

    // Arreglo temporal para invertir
    int* ruta = new int[longitud];
    actual    = nodoDestino;
    for (int i = longitud - 1; i >= 0; i--) {
        ruta[i] = actual;
        actual  = padre[actual];
    }

    // Construir Camino
    Camino camino;
    for (int i = 0; i < longitud; i++) {
        int row, col;
        grafo.getCords(ruta[i], row, col);
        camino.push(col, row);
    }

    delete[] ruta;
    return camino;
}

void Tank::iniciarMovimiento(Camino* camino)
{
    tiempoAcumulado = 0;
    enMovimiento = true;
    //Eliminamos la ruta anterior
    delete rutaPendiente;
    rutaPendiente = camino;
    nodoActual = camino->getCabeza();
    if (nodoActual != nullptr)
        nodoActual = nodoActual->siguiente;  // saltás posición actual
    tiempoAcumulado = 0;
    enMovimiento = true;

}

void Tank::actualizar(float dt)
{

    if (!enMovimiento || nodoActual == nullptr) {
        return;
    }
    if (enMovimiento)
    {
        tiempoAcumulado += dt;
        cout << "Pos: x=" << x << " y=" << y << endl;
        if (tiempoAcumulado >= tiempoMovimiento)
        {
            setPosition(nodoActual->x, nodoActual->y);
            nodoActual = nodoActual->siguiente;
            tiempoAcumulado = 0;

            if (nodoActual == nullptr) enMovimiento = false;
        }
    }
}

Tank::~Tank()
{
    delete rutaPendiente;
}
