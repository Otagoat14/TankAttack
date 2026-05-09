//
// Created by nacho on 5/8/2026.
//
// Map.cpp
// Implementación de la clase Map.
//
// Map es responsable de la generación y administración del mapa de juego.
// Trabaja en conjunto con Graph para establecer las conexiones entre celdas
// y mantiene un arreglo de pesos que describe el costo de transitar cada nodo.
//
// El mapa se genera aleatoriamente con obstáculos según un porcentaje configurable.
// Se garantiza que todas las celdas libres sean accesibles entre sí mediante
// un algoritmo de flood-fill (BFS) que verifica la conectividad del mapa completo.
// Si el mapa generado no es completamente accesible, se regenera automáticamente.
//
// Pesos asignados a cada celda:
//   -1 → obstáculo, sin conexión en el grafo
//    1 → celda libre y transitable
//
// Responsabilidades:
//   makeMapa()      → genera obstáculos aleatoriamente y conecta celdas libres en Graph
//   verificaAcceso()→ BFS desde el primer nodo libre, verifica que todos sean alcanzables
//   doMapa()        → orquesta la generación y regeneración hasta obtener un mapa válido
//   getPeso(nodo)   → retorna el peso de un nodo, usado por Dijkstra para calcular rutas
//
// Dependencias:
//   Graph  → almacena las conexiones entre nodos
//   Cola   → estructura FIFO usada en el flood-fill (definida en Utils.h)

#include "Map.h"
#include <ctime>
#include "Grafo.h"
#include "../Utils.h"
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

void Map::makeMapa()
{
    for (int i = 0; i < rows * cols; i++)
    {
        //Se obtienen las cordenadas del nodo actual
        int fila;
        int colm;
        grafo.getCords(i, fila, colm);

        //Se establece que Nodos seran obstaculos
        if (rand() % 100 < numObstaculos) {
            pesos[i] = -1;
        } else if (rand() % 100 < numObstaculos) {
            //Caminos de barro
            pesos[i] = 2;
        } else if (rand() % 100 < numObstaculos) {
            //Zonas inundadas
            pesos[i] = 5;
        } else {
            //Camino simple
            pesos[i] = 1;
        }
    }
    for (int i = 0; i < rows * cols; i++){
        //Se obtienen las cordenadas del nodo actual
        int fila;
        int colm;
        grafo.getCords(i, fila, colm);

        if (pesos[i] != -1) {  // solo conectar si no es obstáculo

            //Verifica si el vecino de abajo existe y si es accesible
            int nodoVecinoFila = grafo.getNodo(fila + 1, colm);
            if (fila + 1 < rows && getPeso(nodoVecinoFila) != -1) {
                grafo.addVecinos(i, nodoVecinoFila);
            }

            //Verifica si el vecino de la derecha existe y si es accesible
            int nodoVecinoColm = grafo.getNodo(fila, colm + 1);
            if (colm + 1 < cols && getPeso(nodoVecinoColm) != -1) {
                grafo.addVecinos(i, nodoVecinoColm);
            }
        }
        //No se revisa arriba ni a la izquierda dado que es un grafo no dirigido
        ////de esta manera esos nodos ya estan cubiertos al llegar al que se evalua actualmente
    }
}

bool Map::verificaAcceso()
{
    //Primer nodo libre para aplicar "BFS"
    int nInicio;

    //Numero de nodos libres para una comparacion posterior
    int contLibres = 0;

    //Obtener el primer nodo libre
    for (int i = 0; i < rows * cols; i++)
    {
        if (pesos[i] != -1)
        {
            nInicio = i;
            break;
        }
    }

    //Obtener el numero de nodos libres
    for (int i = 0; i < rows * cols; i++)
    {
        if (pesos[i] != -1) {contLibres += 1;}
    }

    //Obtener numero de nodos libres accesibles desde el primer nodo libre
    bool* visitado = new bool[rows * cols];
    for (int i = 0; i < rows * cols; i++) visitado[i] = false;

    //Crea una cola partir del primer nodo libre encontrado
    Cola<int> cola;
    cola.enqueue(nInicio);
    visitado[nInicio] = true;
    int contVecinos = 0;

    //Recorre cada vecino de cada nodo
    while (!cola.empty()) {
        int actual = cola.dequeue();
        //Agrega a un cointador cada vecino visitado
        contVecinos++;

        //Array para los 4 (Maximo) vecinos de cada nodo
        int vecinos[4];
        int numVecinos = 0;
        grafo.getVecinos(actual, vecinos, numVecinos);

        //Agrega a la cola los vecinos encontrados de cada nodo
        for (int i = 0; i < numVecinos; i++) {
            if (!visitado[vecinos[i]]) {
                visitado[vecinos[i]] = true;
                cola.enqueue(vecinos[i]);
            }
        }
    }

    delete[] visitado;
    return contLibres == contVecinos;
}

void Map::doMapa() {
    do {
        grafo.reset();
        //Reinicia los pesos del grafo
        for (int i = 0; i < rows * cols; i++) pesos[i] = 0;
        //Crea una nueva semilla para un nuevo mapa
        srand(time(nullptr));
        makeMapa();
    } while (!verificaAcceso());
}

//Obtiene el Peso de un nodo
int Map::getPeso(int nodo)
{
    return pesos[nodo];
}

//Destructor
Map::~Map() {
    delete[] pesos;
}
