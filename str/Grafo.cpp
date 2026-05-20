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

int Graph::getNodo(int i, int j) const
{
    return i * cols + j;
}

void Graph::getCords(int n, int &i, int &j) const
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

bool Graph:: hasVecino(int u, int v) const
{
    return matrix[u * totalNodes + v];
}

void Graph::getVecinos(int n, int* vecinos, int& numvecinos) const
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

void Graph::reset() {
    for (int i = 0; i < totalNodes * totalNodes; i++) {
        matrix[i] = false;
    }
}

void Graph::bloquearNodo(int node, int* vecinosGuardados, int& numGuardados) {
    numGuardados = 0;
    getVecinos(node, vecinosGuardados, numGuardados);
    for (int i = 0; i < numGuardados; i++) {
        delVecinos(node, vecinosGuardados[i]);
    }
}

void Graph::desbloquearNodo(int node, int* vecinosGuardados, int numGuardados) {
    for (int i = 0; i < numGuardados; i++) {
        addVecinos(node, vecinosGuardados[i]);
    }
}

//Destructor
Graph::~Graph() {
    delete[] matrix;
}
