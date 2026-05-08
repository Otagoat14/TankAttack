#include "TankBFS.h"
#include <iostream>

using namespace std;

tankBFS::tankBFS(int x, int y, int vida, Equipo equipo, Color color)
    : Tank(x, y, vida, equipo, color) {}

void tankBFS::moverse(int nx, int ny, Graph& grafo) {
    Posicion inicio  = {getY(), getX()};
    Posicion destino = {ny, nx};

    Camino camino = BFS(grafo, inicio, destino);

    if (camino.empty()) {
        cout << "No hay camino al destino" << endl;
        return;
    }

    moverPorCeldas(camino, grafo);
}

Camino tankBFS::BFS(Graph& grafo, Posicion inicio, Posicion destino) {
    int totalNodos = grafo.getTotalNodos();

    bool* visitado = new bool[totalNodos];
    int*  padre    = new int[totalNodos];
    for (int i = 0; i < totalNodos; i++) {
        visitado[i] = false;
        padre[i]    = -1;
    }

    int nodoInicio  = grafo.getNodo(inicio.r, inicio.c);
    int nodoDestino = grafo.getNodo(destino.r, destino.c);

    Cola<int> cola;
    cola.enqueue(nodoInicio);
    visitado[nodoInicio] = true;

    bool encontrado = explorarBFS(grafo, cola, visitado, padre, nodoDestino);

    Camino camino;
    if (encontrado)
        camino = reconstruirCamino(grafo, padre, nodoDestino);

    delete[] visitado;
    delete[] padre;
    return camino;
}

bool tankBFS::explorarBFS(Graph& grafo, Cola<int>& cola, bool* visitado, int* padre, int nodoDestino) {
    while (!cola.empty()) {
        int actual = cola.dequeue();

        if (actual == nodoDestino)
            return true;

        int vecinos[4];
        int numVecinos = 0;
        grafo.getVecinos(actual, vecinos, numVecinos);

        for (int i = 0; i < numVecinos; i++) {
            if (!visitado[vecinos[i]]) {
                visitado[vecinos[i]] = true;
                padre[vecinos[i]]    = actual;
                cola.enqueue(vecinos[i]);
            }
        }
    }
    return false;
}

Camino tankBFS::reconstruirCamino(Graph& grafo, int* padre, int nodoDestino) {
    // Primera pasada: contar longitud
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