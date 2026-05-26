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

    T getFrente() const {
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
    // Constructor de copia
    Camino(const Camino& otro) : cabeza(nullptr), cola(nullptr), tamano(0) {
        NodoCamino* actual = otro.cabeza;
        while (actual != nullptr) {
            push(actual->x, actual->y);
            actual = actual->siguiente;
        }
    }

    Camino& operator=(const Camino& otro) {
        if (this == &otro) return *this;
        NodoCamino* actual = cabeza;
        while (actual != nullptr) {
            NodoCamino* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        cabeza = nullptr;
        cola   = nullptr;
        tamano = 0;

        actual = otro.cabeza;
        while (actual != nullptr) {
            push(actual->x, actual->y);
            actual = actual->siguiente;
        }
        return *this;
    }

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

struct Punto {
    int x, y;
    Punto(int x = -1, int y = -1) : x(x), y(y) {}
};


struct NodoPQ {
    int nodo;
    int costo;
    NodoPQ* siguiente;
    NodoPQ(int nodo, int costo) : nodo(nodo), costo(costo), siguiente(nullptr) {}
};

class ColaPrioridad {
    NodoPQ* cabeza;

public:
    ColaPrioridad() : cabeza(nullptr) {}

    ~ColaPrioridad() {
        while (cabeza != nullptr) {
            NodoPQ* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void insertar(int nodo, int costo) {
        NodoPQ* nuevo = new NodoPQ(nodo, costo);
        // Lista ordenada por costo ascendente
        if (cabeza == nullptr || costo < cabeza->costo) {
            nuevo->siguiente = cabeza;
            cabeza = nuevo;
            return;
        }
        NodoPQ* actual = cabeza;
        while (actual->siguiente != nullptr && actual->siguiente->costo <= costo)
            actual = actual->siguiente;
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }

    int extraerMin() {
        if (cabeza == nullptr) return -1;
        NodoPQ* temp = cabeza;
        int nodo = temp->nodo;
        cabeza = cabeza->siguiente;
        delete temp;
        return nodo;
    }

    bool empty() const { return cabeza == nullptr; }
};

//Cambiar el vector por el grafo
inline bool lineaVista(int x0, int y0, int x1, int y1, const Graph& grafo) {
    if (x0 == x1 && y0 == y1)
        return grafo.esPasable(y0, x0);

    int dx    = abs(x1 - x0);
    int dy    = abs(y1 - y0);
    int dirx  = (x0 < x1) ? 1 : -1;
    int diry  = (y0 < y1) ? 1 : -1;
    int error = dx - dy;
    int x = x0, y = y0;

    while (true) {
        if (!grafo.esPasable(y, x))
            return false;
        if (x == x1 && y == y1)
            return true;

        int error2 = error * 2;
        if (error2 > -dy) { error -= dy; x += dirx; }
        if (error2 < dx)  { error += dx; y += diry; }
    }
}

inline Camino celdаsLineaVista(int x0, int y0, int x1, int y1, const Graph& grafo) {
    Camino camino;

    if (x0 == x1 && y0 == y1) {
        if (grafo.esPasable(y0, x0))
            camino.push(x0, y0);
        return camino;
    }

    int dx  = abs(x1 - x0);
    int dy  = abs(y1 - y0);
    int sx  = (x0 < x1) ? 1 : -1;
    int sy  = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    int x = x0, y = y0;

    while (true) {
        if (!grafo.esPasable(y, x))
            break;
        camino.push(x, y);
        if (x == x1 && y == y1)
            break;

        int err2 = err * 2;
        if (err2 > -dy) { err -= dy; x += sx; }
        if (err2 < dx)  { err += dx; y += sy; }
    }

    return camino;
}


// ── Lista simple para rutas visibles ──────────────────────────────────────
struct NodoRuta {
    int col, fila;
    NodoRuta* siguiente;
    NodoRuta(int c, int f) : col(c), fila(f), siguiente(nullptr) {}
};

class ListaRuta {
    NodoRuta* cabeza;
    NodoRuta* cola;

public:
    ListaRuta() : cabeza(nullptr), cola(nullptr) {}

    ~ListaRuta() { limpiar(); }

    void limpiar() {
        NodoRuta* actual = cabeza;
        while (actual != nullptr) {
            NodoRuta* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        cabeza = nullptr;
        cola   = nullptr;
    }

    void agregar(int col, int fila) {
        NodoRuta* nuevo = new NodoRuta(col, fila);
        if (cola == nullptr) {
            cabeza = nuevo;
            cola   = nuevo;
        } else {
            cola->siguiente = nuevo;
            cola = nuevo;
        }
    }

    NodoRuta* getCabeza() const { return cabeza; }
    bool vacia() const { return cabeza == nullptr; }
};


// ── Power-ups ─────────────────────────────────────────────────────────────
enum class TipoPowerUp {
    DOBLE_TURNO,
    PRECISION_MOVIMIENTO,
    PRECISION_ATAQUE,
    PODER_ATAQUE
};

// ── A* ────────────────────────────────────────────────────────────────────
inline Camino aStar(Graph& grafo, Posicion inicio, Posicion destino) {
    int totalNodos = grafo.getTotalNodos();

    int*  gCost   = new int[totalNodos];
    int*  hCost   = new int[totalNodos];
    int*  fCost   = new int[totalNodos];
    int*  padre   = new int[totalNodos];
    bool* abierto = new bool[totalNodos];
    bool* cerrado = new bool[totalNodos];

    for (int i = 0; i < totalNodos; i++) {
        gCost[i]   = 99999;
        hCost[i]   = 0;
        fCost[i]   = 99999;
        padre[i]   = -1;
        abierto[i] = false;
        cerrado[i] = false;
    }

    int nodoInicio  = grafo.getNodo(inicio.r,   inicio.c);
    int nodoDestino = grafo.getNodo(destino.r, destino.c);

    // Heuristica Manhattan
    auto heuristica = [&](int nodo) -> int {
        int r, c;
        grafo.getCords(nodo, r, c);
        return abs(r - destino.r) + abs(c - destino.c);
    };

    gCost[nodoInicio]   = 0;
    hCost[nodoInicio]   = heuristica(nodoInicio);
    fCost[nodoInicio]   = hCost[nodoInicio];
    abierto[nodoInicio] = true;

    bool encontrado = false;

    while (true) {
        // Encontrar nodo abierto con menor fCost
        int actual = -1;
        for (int i = 0; i < totalNodos; i++) {
            if (abierto[i] && !cerrado[i]) {
                if (actual == -1 || fCost[i] < fCost[actual])
                    actual = i;
            }
        }

        if (actual == -1) break;  // no hay camino

        if (actual == nodoDestino) { encontrado = true; break; }

        abierto[actual] = false;
        cerrado[actual] = true;

        int vecinos[4];
        int numVecinos = 0;
        grafo.getVecinos(actual, vecinos, numVecinos);

        for (int i = 0; i < numVecinos; i++) {
            int v = vecinos[i];
            if (cerrado[v]) continue;

            int nuevoG = gCost[actual] + 1;
            if (nuevoG < gCost[v]) {
                gCost[v]   = nuevoG;
                hCost[v]   = heuristica(v);
                fCost[v]   = gCost[v] + hCost[v];
                padre[v]   = actual;
                abierto[v] = true;
            }
        }
    }

    // Reconstruir camino manualmente (no podemos llamar Tank::reconstruirCamino)
    Camino camino;
    if (encontrado) {
        int longitud = 0;
        int actual   = nodoDestino;
        while (actual != -1) { longitud++; actual = padre[actual]; }

        int* ruta = new int[longitud];
        actual    = nodoDestino;
        for (int i = longitud - 1; i >= 0; i--) {
            ruta[i] = actual;
            actual  = padre[actual];
        }
        for (int i = 0; i < longitud; i++) {
            int r, c;
            grafo.getCords(ruta[i], r, c);
            camino.push(c, r);
        }
        delete[] ruta;
    }

    delete[] gCost;
    delete[] hCost;
    delete[] fCost;
    delete[] padre;
    delete[] abierto;
    delete[] cerrado;

    return camino;
}
#endif