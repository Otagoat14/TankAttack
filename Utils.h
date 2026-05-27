#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include "str/Grafo.h"

using namespace std;

// Representa una posicion en el grafo (fla, columna)
struct Posicion {
    int r, c;
};

// Cola generica FIFO implementada con lista enlazada
template <typename T>

class Cola {
    struct Nodo {
        T dato;
        Nodo* siguiente;

        Nodo(T dato) : dato(dato), siguiente(nullptr) {}
    };

    Nodo* frente;  // Primer elemento de la cola
    Nodo* final;   // Ultimo elemento de la cola
    int tamano;

    public:

    Cola() : frente(nullptr), final(nullptr), tamano(0) {}

    // Agrega un elemento al final de la cola
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

    // Elimina y retorna el elemento del frente de la cola
    T dequeue() {
        if (empty()) {
            cout << "La cola esta vacia" << endl;
        }
        Nodo* nodoEliminar = frente;
        T dato = frente->dato;
        frente = frente->siguiente;

        // Si era el ultimo elemento, final tambien queda en null
        if (frente == nullptr) {
            final = nullptr;
        }

        delete nodoEliminar;
        tamano--;
        return dato;
    }

    // Retorna el elemento del frente sin eliminarlo
    T getFrente() const {
        if (empty()) {
            cout << "La cola esta vacia" << endl;
        }
        return frente->dato;
    }

    // Retorna true si la cola no tiene elementos
    bool empty() const {
        return frente == nullptr;
    }

    // Retorna el numero de elementos en la cola
    int getTamano() const {
        return tamano;
    }

};

// Retorna el numero de elementos en la cola
struct NodoCamino {
    int x, y;
    NodoCamino* siguiente;
    NodoCamino(int x, int y) : x(x), y(y), siguiente(nullptr) {}
};

// Lista enlazada que representa un camino como secuencia de coordenadas
class Camino
{
    NodoCamino* cabeza;
    NodoCamino* cola;
    int tamano;

public:
    Camino() : cabeza(nullptr), cola(nullptr), tamano(0) {}
    // Constructor de copia, recorre el camino generando una copia de cada nodo
    Camino(const Camino& otro) : cabeza(nullptr), cola(nullptr), tamano(0) {
        NodoCamino* actual = otro.cabeza;
        while (actual != nullptr) {
            push(actual->x, actual->y);
            actual = actual->siguiente;
        }
    }

    // Operador sobrecargado libera el camino actual y copia el nuevo
    Camino& operator=(const Camino& otro) {
        if (this == &otro) return *this;

        // Libera los nodos del camino actual
        NodoCamino* actual = cabeza;
        while (actual != nullptr) {
            NodoCamino* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        cabeza = nullptr;
        cola   = nullptr;
        tamano = 0;

        // Copia los nodos del camino origen
        actual = otro.cabeza;
        while (actual != nullptr) {
            push(actual->x, actual->y);
            actual = actual->siguiente;
        }
        return *this;
    }

    // Libera todos los nodos del camino
    ~Camino() {
        NodoCamino* actual = cabeza;
        while (actual != nullptr) {
            NodoCamino* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }

    // Agrega una coordenada al final del camino
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
    // Retorna el primer nodo del camino
    NodoCamino* getCabeza() const { return cabeza; }
    // Retorna el numero de pasos en el camino
    int getTamano() const { return tamano; }
    // Retorna true si el camino no tiene pasos
    bool empty() const { return cabeza == nullptr; }
};

//Representa un punto en el grafo originalmente invalido
struct Punto {
    int x, y;
    Punto(int x = -1, int y = -1) : x(x), y(y) {}
};

// Nodo de la cola de prioridad con su costo asociado
struct NodoPQ {
    int nodo;
    int costo;
    NodoPQ* siguiente;
    NodoPQ(int nodo, int costo) : nodo(nodo), costo(costo), siguiente(nullptr) {}
};

// Cola de prioridad minima implementada con lista enlazada ordenada por costo
// Se usa en Dijkstra para extraer siempre el nodo de menor costo
class ColaPrioridad {
    NodoPQ* cabeza;

public:
    ColaPrioridad() : cabeza(nullptr) {}

    // Libera todos los nodos de la cola
    ~ColaPrioridad() {
        while (cabeza != nullptr) {
            NodoPQ* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    // Inserta un nodo manteniendo el orden ascendente por costo
    void insertar(int nodo, int costo) {
        NodoPQ* nuevo = new NodoPQ(nodo, costo);
        // Lista ordenada por costo ascendente
        if (cabeza == nullptr || costo < cabeza->costo) {
            nuevo->siguiente = cabeza;
            cabeza = nuevo;
            return;
        }
        // Busca la posicion correcta para mantener el orden
        NodoPQ* actual = cabeza;
        while (actual->siguiente != nullptr && actual->siguiente->costo <= costo)
            actual = actual->siguiente;
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }
    // Extrae y retorna el nodo con menor costo
    int extraerMin() {
        if (cabeza == nullptr) return -1;
        NodoPQ* temp = cabeza;
        int nodo = temp->nodo;
        cabeza = cabeza->siguiente;
        delete temp;
        return nodo;
    }

    // Retorna true si la cola no tiene elementos
    bool empty() const { return cabeza == nullptr; }
};


// Retorna true si todos los nodos en la linea son pasables
inline bool lineaVista(int x0, int y0, int x1, int y1, const Graph& grafo) {
    if (x0 == x1 && y0 == y1)
        return grafo.esPasable(y0, x0);

    int dx    = abs(x1 - x0);
    int dy    = abs(y1 - y0);
    int dirx  = (x0 < x1) ? 1 : -1;  // Direccion horizontal
    int diry  = (y0 < y1) ? 1 : -1;  // Direccion vertical
    int error = dx - dy;             // Acumulador del error
    int x = x0, y = y0;

    while (true) {
        if (!grafo.esPasable(y, x))
            return false;
        if (x == x1 && y == y1)
            return true;

        // Ajusta el error y avanza en la direccion correspondiente
        int error2 = error * 2;
        if (error2 > -dy) { error -= dy; x += dirx; }
        if (error2 < dx)  { error += dx; y += diry; }
    }
}

// Igual que lineaVista pero retorna el camino de celdas recorridas hasta
// encontrar un obstaculo o llegar al destino
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
        // Si encuentra un obstaculo detiene el recorrido
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

// Nodo de la lista de ruta con coordenadas de columna y fila
struct NodoRuta {
    int col, fila;
    NodoRuta* siguiente;
    NodoRuta(int c, int f) : col(c), fila(f), siguiente(nullptr) {}
};

// Lista enlazada simple para almacenar una ruta de celdas visibles
class ListaRuta {
    NodoRuta* cabeza;
    NodoRuta* cola;

public:
    ListaRuta() : cabeza(nullptr), cola(nullptr) {}

    ~ListaRuta() { limpiar(); }

    // Libera todos los nodos de la lista
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

    // Agrega una celda al final de la ruta
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

    // Retorna el primer nodo de la ruta
    NodoRuta* getCabeza() const { return cabeza; }
    // Retorna true si la ruta no tiene nodos
    bool vacia() const { return cabeza == nullptr; }
};


// Tipos de power-up disponibles en el juego
enum class TipoPowerUp {
    DOBLE_TURNO,           // Otorga 2 turnos extra al jugador
    PRECISION_MOVIMIENTO,  // El siguiente movimiento ignora el costo del terreno
    PRECISION_ATAQUE,      // El siguiente disparo tiene mayor precision
    PODER_ATAQUE           // El siguiente disparo tiene mayor dano
};

// Algoritmo A* para encontrar el camino de menor costo entre inicio y destino
//Heuritaca Manhattan
inline Camino aStar(Graph& grafo, Posicion inicio, Posicion destino) {
    int totalNodos = grafo.getTotalNodos();

    int*  gCost   = new int[totalNodos];   // Costo real desde el inicio
    int*  hCost   = new int[totalNodos];   // Costo estimado al destino
    int*  fCost   = new int[totalNodos];   // gCost + hCost
    int*  padre   = new int[totalNodos];   // Nodo anterior en el camino optimo
    bool* abierto = new bool[totalNodos];  // Nodos por evaluar
    bool* cerrado = new bool[totalNodos];  // Nodos ya evaluados

    // Inicializa todos los costos en infinito y sin padre
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

            // Solo actualiza si encontro un camino mas corto al vecino
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

        // Llena el arreglo de atras hacia adelante para obtener el orden correcto
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