//
// Created by nacho on 5/8/2026.
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

//Inicializa el mapa con el grafo,
//establece dimenciones y porcentaje de obstaculos
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

//Genera el mapa
//El mapa se genera en dos pasadas
void Map::makeMapa()
{
    //Primera pasada asigna aleatoriamente el tipo de terreno a cada celda
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
    //Segunda pasada conecta en el grafo los nodos adyacentes que no sean obstáculos
    // Solo revisa el vecino de abajo y el de la derecha
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
        //de esta manera esos nodos ya estan cubiertos al llegar al que se evalua actualmente
    }
}

//Verifica que todos los nodos transitables sean accesibles
bool Map::verificaAcceso()
{
        int nInicio = getPrimerNodoLibre();
        if (nInicio == -1) return false; // Todo el mapa es obstáculos

        //Numero de nodos libres para una comparacion posterior
        int libres     = contarNodosLibres();
        int alcanzados = contarAlcanzables(nInicio);

        return libres == alcanzados;
}

// Retorna el índice del primer nodo libre encontrado
int Map::getPrimerNodoLibre() {
    //Primer nodo libre para aplicar algoritmo
    for (int i = 0; i < rows * cols; i++) {
        if (pesos[i] != -1) return i;
    }
    return -1;
}

// Cuenta cuántos nodos no son obstáculos
int Map::contarNodosLibres() {
    //Numero de nodos libres para una comparacion posterior
    int cont = 0;
    for (int i = 0; i < rows * cols; i++) {
        if (pesos[i] != -1) cont++;
    }
    return cont;
}

// BFS desde nInicio, retorna cuántos nodos fueron alcanzados
int Map::contarAlcanzables(int nInicio) {
    //Obtener numero de nodos libres accesibles desde el primer nodo libre
    bool* visitado = new bool[rows * cols];
    for (int i = 0; i < rows * cols; i++) visitado[i] = false;

    //Crea una cola a partir del primer nodo libre encontrado
    Cola<int> cola;
    cola.enqueue(nInicio);
    visitado[nInicio] = true;
    int contVecinos = 0;

    //Recorre cada vecino de cada nodo
    while (!cola.empty()) {
        int actual = cola.dequeue();
        //Agrega a un contador cada vecino visitado
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
    return contVecinos;
}

// Regenera hasta hacer un mapa valido
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
