#ifndef UTILS_H
#define UTILS_H

#include <iostream>
using namespace std;

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
inline bool lineaVista(int x0, int y0, int x1, int y1, const vector<vector<int>>& matriz) {

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
        if (matriz[y][x] == false) {
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
inline vector<pair<int,int>> celdасLineaVista(int x0, int y0, int x1, int y1, const vector<vector<bool>>& matriz) {
    vector<pair<int,int>> celdas;

    int dx  = abs(x1 - x0);
    int dy  = abs(y1 - y0);
    int sx  = (x0 < x1) ? 1 : -1;
    int sy  = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int x = x0;
    int y = y0;

    while (true) {
        if (!matriz[y][x]) {
            break;
        }
        celdas.push_back({x, y});

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

    return celdas;
};
#endif