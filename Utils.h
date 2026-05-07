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

#endif