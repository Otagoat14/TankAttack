#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include "str/Grafo.h"

using namespace std;

struct Posicion {
    int r, c;
};

template <typename T>

class Cola {
    struct Nodo {
        T dato;
        Nodo* siguiente;

        Nodo(T dato) : dato(dato), siguiente(nullptr) {}
    };

    Nodo* frente;
    Nodo* final;
    int tamano;

    public:

    Cola() : frente(nullptr), final(nullptr), tamano(0) {}

    void enqueue(T dato) {
        Nodo* nuevoNodo = new Nodo(dato);
        if (empty()) {
            frente = nuevoNodo;
            final = nuevoNodo;
        }
        else {
            final->siguiente = nuevoNodo;
            final = nuevoNodo;
        }
        tamano++;
    }

    T dequeue() {
        if (empty()) {
            cout << "La cola esta vacia" << endl;
        }
        Nodo* nodoEliminar = frente;
        T dato = frente->dato;
        frente = frente->siguiente;

        if (frente == nullptr) {
            final = nullptr;
        }

        delete nodoEliminar;
        tamano--;
        return dato;
    }

    T getFrente() {
        if (empty()) {
            cout << "La cola esta vacia" << endl;
        }
        return frente->dato;
    }

    bool empty() const {
        return frente == nullptr;
    }

    int getTamano() const {
        return tamano;
    }

};

struct NodoCamino {
    int x, y;
    NodoCamino* siguiente;
    NodoCamino(int x, int y) : x(x), y(y), siguiente(nullptr) {}
};

class Camino {
    NodoCamino* cabeza;
    NodoCamino* cola;
    int tamano;

public:
    Camino() : cabeza(nullptr), cola(nullptr), tamano(0) {}

    ~Camino() {
        NodoCamino* actual = cabeza;
        while (actual != nullptr) {
            NodoCamino* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }

    void push(int x, int y) {
        NodoCamino* nuevo = new NodoCamino(x, y);
        if (cola == nullptr) {
            cabeza = nuevo;
            cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            cola = nuevo;
        }
        tamano++;
    }

    NodoCamino* getCabeza() const { return cabeza; }
    int getTamano() const { return tamano; }
    bool empty() const { return cabeza == nullptr; }
};

//Cambiar el vector por el grafo
inline bool lineaVista(int x0, int y0, int x1, int y1, const Graph& grafo) {

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    //Estas son las direcciones en que se puede mover
    int dirx = (x0 < x1) ? 1 : -1;  
    int diry = (y0 < y1) ? 1 : -1;
    int error = dx - dy;

    int x = x0;
    int y = y0;

    while (true) {

        //Obstaculo
        if (!grafo.esPasable(y, x)) {
            return false;
        }

        //Llego
        if (x == x1 && y == y1) {
            return true;
        }

        // Avanzar en x o en y
        int error2 = error * 2;

        if (error2 > -dy) {
            error -= dy;
            x += dirx;
        }

        if (error2 < dx) {
            error += dx;
            y += diry;
        }
    }
}

//Cree la funciom para retornar las celdas que conforman la linea vista, para que asi el tanque se pueda mover
//Adaptar para hacer luego con el grafo
inline Camino celdаsLineaVista(int x0, int y0, int x1, int y1, const Graph& grafo) {
    Camino camino;

    int dx  = abs(x1 - x0);
    int dy  = abs(y1 - y0);
    int sx  = (x0 < x1) ? 1 : -1;
    int sy  = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int x = x0;
    int y = y0;

    while (true) {
        if (!grafo.esPasable(y, x)) {
            break;
        }
        camino.push(x, y);

        if (x == x1 && y == y1) {
            break;
        }

        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy; x += sx;
        }
        if (err2 < dx) {
            err += dx; y += sy;
        }
    }

    return camino;
};
#endif