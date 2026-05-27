//
// Created by nacho on 4/30/2026.
//
#include "Grafo.h"

//Aloca la matriz en 1d para mayor eficiencia
Graph::Graph(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    this->totalNodes = rows * cols;

    //Al ser una matriz 1d hay un numero asignado para cada par ordenado
    //por esto necesita totalNodes cuadrado
    matrix = new bool[totalNodes * totalNodes];

    // Iniciamos los nodos de la matriz en 0 para indicar que no hay vecinos establecidos
    for (int i = 0; i < totalNodes * totalNodes; i++)
    {
        matrix [i] = false;                                  
    }
}

//Formula para obtener el indice de un par ordenado
int Graph::getNodo(int i, int j) const
{
    return i * cols + j;
}

// Obtiene el par ordenado de un indice
void Graph::getCords(int n, int &i, int &j) const
{
    //formula de aplanamiento i * cols
    // invirtiendola recuperamos i
    i = n / cols;
    //furmula de aplanamiento +j
    //el sobrante del modulo es el valor de la suma
    j = n % cols;
}

//Establece en la matriz de adyacencia quienes son vecinos
void Graph::addVecinos(int n1, int n2) {
    //Al ser bidereccional le añade el vecino a cada nodo
    //Pone al vecino n2 accesible para n1
    matrix [n1 * totalNodes + n2] = true;
    //Pone al vecino n1 accesible para n2
    matrix [n2 * totalNodes + n1] = true;

}

//Establece en la matriz de adyacencia quienes no son vecinos
void Graph::delVecinos(int n1, int n2) {
    //Al ser bidereccional elimina el vecino a cada nodo
    //Pone al vecino n2 inaccesible para n1
    matrix [n1 * totalNodes + n2] = false;
    //Pone al vecino n1 inaccesible para n2
    matrix [n2 * totalNodes + n1] = false;

}

//Retorna si un nodo esta en el arreglo
bool Graph::isNodo(int u, int v) const
{
    return matrix[u * totalNodes + v];
}

//Obtiene los 4 posibles vecinos de un nodo
void Graph::getVecinos(int n, int* vecinos, int& numvecinos) const
{
    // Columnas y filas
    int i,j;
    //Obtienen las cordenadas de nodo a analizar
    getCords(n, i, j);

    //Establece cuales son los posibles nodos vecinos
    int vecinoArriba = getNodo(i - 1, j);
    int vecinoAbajo = getNodo(i + 1, j);
    int vecinoIzq = getNodo(i, j - 1);
    int vecinoDer = getNodo(i, j + 1);

    // Revisa si es un borde y valida que esten en el arreglo
    //Una vez hechas las validaciones asigna los nodos vecinos
    //Ademas cuanta el numero de vecinos asignados
    if (i - 1 >= 0 && isNodo(n, vecinoArriba)) {
        vecinos[numvecinos] = vecinoArriba;
        numvecinos++;
    }
    if (i + 1 < rows && isNodo(n, vecinoAbajo)) {
        vecinos[numvecinos] = vecinoAbajo;
        numvecinos++;
    }
    if (j - 1 >= 0 && isNodo(n, vecinoIzq)) {
        vecinos[numvecinos] = vecinoIzq;
        numvecinos++;
    }
    if (j + 1 < cols && isNodo(n, vecinoDer)) {
        vecinos[numvecinos] = vecinoDer;
        numvecinos++;
    }
}

//Verifica si un nodo es trancitable consultado si tiene vecinos
bool Graph::esPasable(int row, int col) const {
        int nodo = getNodo(row, col);
    //Reserva de forma temporal memoria para el funcionamiento de getVecinos
    //Pero estos como tal no nos interesan por eso se usa el heap
        int* vecinos = new int[4];
        int numVecinos = 0;
        getVecinos(nodo, vecinos, numVecinos);
        delete[] vecinos;
        return numVecinos > 0;
}

//Elimiana todas las conexines entre nodos
void Graph::reset() {
    for (int i = 0; i < totalNodes * totalNodes; i++) {
        matrix[i] = false;
    }
}

//Establece un nodo como inaccesible (añade todos los vecinos)
void Graph::bloquearNodo(int node, int* vecinosGuardados, int& numGuardados) {
    numGuardados = 0;
    getVecinos(node, vecinosGuardados, numGuardados);
    for (int i = 0; i < numGuardados; i++) {
        delVecinos(node, vecinosGuardados[i]);
    }
}

//Establece un nodo como accesible
void Graph::desbloquearNodo(int node, int* vecinosGuardados, int numGuardados) {
    for (int i = 0; i < numGuardados; i++) {
        //Verifica que el nodo a conectar si sea accesible
        int fila, col;
        getCords(vecinosGuardados[i], fila, col);
        if (esPasable(fila, col)) {
            addVecinos(node, vecinosGuardados[i]);
        }
        addVecinos(node, vecinosGuardados[i]);
    }
}

//Destructor
Graph::~Graph() {
    delete[] matrix;
}
