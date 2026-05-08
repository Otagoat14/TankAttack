//
// Created by nacho on 4/30/2026.
//
// Grafo.h
// Clase Graph: representa el mapa del juego como un grafo no dirigido
// mediante una matriz de adyacencia implementada como arreglo 1D en heap.
//
// Cada celda (row, col) de la grilla se mapea a un índice único:
//   índice = row * cols + col
//
// La matriz de adyacencia es de tamaño (totalNodes * totalNodes).
// matrix[u * totalNodes + v] == true indica que hay arista entre nodo u y nodo v.
//
// Responsabilidad: almacenar conexiones entre nodos únicamente.
// No conoce obstáculos ni lógica del juego — eso es responsabilidad otras clases

#ifndef TANKATTACK_GRAFO_H
#define TANKATTACK_GRAFO_H

class Graph {
private:
    int rows;
    int cols;
    int totalNodes;
    bool* matrix;

public:
    Graph(int rows, int cols);
    ~Graph();

    //Añade vecinos a un nodo
    void addVecinos(int u, int v);

    //Elimina Vecinos de un nodo
    void delVecinos(int u, int v);

    // Retorna los vecinos de un nodo
    void getVecinos(int node, int* neighbors, int& count);

    // Convierte coordenadas (row, col) a índice del nodo
    int getNodo(int row, int col) const;

    // Convierte índice de nodo a coordenadas
    void getCords(int n, int &row, int &col);

    //Revisa si tiene vecinos asignados
    bool hasVecino(int u, int v);

    //Me dice si es obstaculo
    bool esPasable(int row, int col) const;

    //Devuelve el total de nodos
    int getTotalNodos() const { return totalNodes; }
};

#endif //TANKATTACK_GRAFO_H