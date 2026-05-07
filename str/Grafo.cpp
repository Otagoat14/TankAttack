//
// Created by nacho on 4/30/2026.
//
// Grafo.cpp
// Implementación de la clase Graph.
//
// Constructor: aloca la matriz en heap e inicializa en 0 / false
// Destructor: libera la memoria alocada.
//
// addVecinos(u, v): conecta dos nodos en ambas direcciones (no dirigido).
// delVecinos(u, v): elimina la conexión entre dos nodos.
// hasVecino(u, v): retorna true si hay arista entre u y v.
// getNodo(i, j): convierte coordenadas (row, col) a índice de nodo.
// getCords(n, i, j): convierte índice de nodo a coordenadas (row, col).
// getVecinos(n, vecinos, count): llena el arreglo vecinos con los nodos
//   adyacentes a n que tengan arista activa, y retorna su cantidad en count.

#include "Grafo.h"

Graph::Graph(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    this->totalNodes = rows * cols;

    matrix = new bool[totalNodes * totalNodes];

    // Iniciamos los nodos de la matriz en 0 para indicar que no hay vecinos establecidos
    for (int i = 0; i < totalNodes * totalNodes; i++)
    {
        matrix [i] = false;                                  
    }
}

int Graph::getNodo(int i, int j)
{
    return i * cols + j;
}

void Graph::getCords(int n, int &i, int &j)
{
    i = n / cols;
    j = n % cols;
}

void Graph::addVecinos(int n1, int n2) {

    matrix [n1 * totalNodes + n2] = true;
    matrix [n2 * totalNodes + n1] = true;

}

void Graph::delVecinos(int n1, int n2) {

    matrix [n1 * totalNodes + n2] = false;
    matrix [n2 * totalNodes + n1] = false;

}

bool Graph:: hasVecino(int u, int v)
{
    return matrix[u * totalNodes + v];
}

void Graph::getVecinos(int n, int* vecinos, int& numvecinos)
{
    // Columnas y filas
    int i,j;
    getCords(n, i, j);

    int vecinoArriba = getNodo(i - 1, j);
    int vecinoAbajo = getNodo(i + 1, j);
    int vecinoIzq = getNodo(i, j - 1);
    int vecinoDer = getNodo(i, j + 1);

    // Revisa si es un borde
    if (i - 1 >= 0 && hasVecino(n, vecinoArriba)) {
        vecinos[numvecinos] = vecinoArriba;
        numvecinos++;
    }
    if (i + 1 < rows && hasVecino(n, vecinoAbajo)) {
        vecinos[numvecinos] = vecinoAbajo;
        numvecinos++;
    }
    if (j - 1 >= 0 && hasVecino(n, vecinoIzq)) {
        vecinos[numvecinos] = vecinoIzq;
        numvecinos++;
    }
    if (j + 1 < cols && hasVecino(n, vecinoDer)) {
        vecinos[numvecinos] = vecinoDer;
        numvecinos++;
    }
}

bool Graph::esPasable(int row, int col) const {
    int nodo = getNodo(row, col);
    for (int i = 0; i < totalNodes; i++) {
        if (matrix[nodo * totalNodes + i]) {
            return true;
        }
    }
    return false;
}

//Destructor
Graph::~Graph() {
    delete[] matrix;
}

//Main de prueba Eliminar tras depuracion
#include <iostream>

int main() {
    Graph g(4, 4);

    // Conectar (0,0) con (0,1) y (1,0)
    g.addVecinos(g.getNodo(0,0), g.getNodo(0,1));
    g.addVecinos(g.getNodo(0,0), g.getNodo(1,0));

    // Verificar vecinos de (0,0)
    int vecinos[4];
    int count = 0;
    g.getVecinos(g.getNodo(0,0), vecinos, count);

    std::cout << "Vecinos de (0,0): " << count << std::endl;
    for (int i = 0; i < count; i++) {
        std::cout << vecinos[i] << std::endl;
    }

    // Debería imprimir 2 vecinos: nodo 1 y nodo 4
    return 0;
}