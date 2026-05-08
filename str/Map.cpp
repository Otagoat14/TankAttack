//
// Created by nacho on 5/8/2026.
//

#include "Map.h"
#include "Grafo.h"
#include <iostream>
using namespace std;

Map::Map(Graph& grafo, int rows, int cols, int porcentaje)
    // No se puede asignar directamente
    // se necesita inicializar
    : grafo(grafo) {

    this->rows = rows;
    this->cols = cols;
    this->numObstaculos = porcentaje;

    //Se guarda el espacio para almacenar los datos
    //de los pesos al igual que como se hizo con la matriz del grafo
    pesos = new int[rows * cols];
}

void Map::generateMap(){
    for (int i = 0; i < rows * cols; i++)
    {
        int fila;
        int colm;
        grafo.getCords(i, fila, colm);

        if (rand() % 100 < numObstaculos) {
            pesos[i] = -1;
        } else
        {
            //Agregar Otro tipo de seleccion aleatoria para
            //Obstaculos transitables

            pesos[i] = 1;

            int nodoVecinoFila = grafo.getNodo(fila + 1, colm);
            if (fila + 1 < rows && getPeso(nodoVecinoFila) != -1) {
                grafo.addVecinos(i, nodoVecinoFila);
            }

            int nodoVecinoColm = grafo.getNodo(fila, colm + 1);
            if (colm + 1 < cols && getPeso(nodoVecinoColm) != -1) {
                grafo.addVecinos(i, nodoVecinoColm);
            }
        }
    }
}

int Map::getPeso(int nodo)
{
    return pesos[nodo];
}

Map::~Map() {
    delete[] pesos;
}

void imprimirMapa(Map& map, Graph& grafo, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int nodo = grafo.getNodo(i, j);
            if (map.getPeso(nodo) == -1)
                cout << "# ";
            else
                cout << ". ";
        }
        cout << endl;
    }
}
