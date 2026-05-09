#include <iostream>
#include "str/Grafo.h"
#include "Personajes/Tank.h"
#include "Personajes/TankBFS.h"
#include "Personajes/TankDijkstra.h"

using namespace std;

// Construye un grafo de filas x cols conectando todas las celdas libres
// obstacles es un arreglo de pares (row, col) que representan obstáculos
void construirGrafo(Graph& grafo, int filas, int cols, int* obsRows, int* obsCols, int numObs) {

    // Marcar celdas bloqueadas
    bool* bloqueado = new bool[filas * cols];
    for (int i = 0; i < filas * cols; i++)
        bloqueado[i] = false;

    for (int i = 0; i < numObs; i++)
        bloqueado[obsRows[i] * cols + obsCols[i]] = true;

    // Conectar celdas libres con sus vecinos libres
    for (int r = 0; r < filas; r++) {
        for (int c = 0; c < cols; c++) {
            if (bloqueado[r * cols + c]) continue;

            int nodo = grafo.getNodo(r, c);

            // Vecino derecho
            if (c + 1 < cols && !bloqueado[r * cols + (c + 1)])
                grafo.addVecinos(nodo, grafo.getNodo(r, c + 1));

            // Vecino abajo
            if (r + 1 < filas && !bloqueado[(r + 1) * cols + c])
                grafo.addVecinos(nodo, grafo.getNodo(r + 1, c));
        }
    }

    delete[] bloqueado;
}

void imprimirGrafo(Graph& grafo, int filas, int cols, int xBFS, int yBFS, int xDijk, int yDijk) {
    for (int r = 0; r < filas; r++) {
        for (int c = 0; c < cols; c++) {
            int nodo = grafo.getNodo(r, c);
            int vecinos[4];
            int count = 0;
            grafo.getVecinos(nodo, vecinos, count);

            if (r == yBFS && c == xBFS)
                cout << " B ";
            else if (r == yDijk && c == xDijk)
                cout << " D ";
            else if (count == 0)
                cout << " # ";
            else
                cout << " . ";
        }
        cout << endl;
    }
}

int main() {
    const int FILAS = 6;
    const int COLS  = 6;

    // Obstáculos en (row, col)
    int obsRows[] = {1, 1, 2, 3, 3};
    int obsCols[] = {2, 3, 2, 2, 3};
    int numObs    = 5;

    Graph grafo(FILAS, COLS);
    construirGrafo(grafo, FILAS, COLS, obsRows, obsCols, numObs);

    // BFS empieza en (col=0, row=0), destino (col=5, row=5)
    tankBFS tanqueBFS(1, 1, 100, Equipo::JUGADOR1, Color::AZUL);

    // Dijkstra empieza en (col=5, row=0), destino (col=0, row=5)
    tankDijkstra tanqueDijkstra(5, 0, 100, Equipo::JUGADOR2, Color::ROJO);

    cout << "=== Mapa inicial ===" << endl;
    cout << "B = TankBFS, D = TankDijkstra, # = obstaculo, . = libre" << endl;
    imprimirGrafo(grafo, FILAS, COLS, tanqueBFS.getX(), tanqueBFS.getY(), tanqueDijkstra.getX(), tanqueDijkstra.getY());

    cout << "\n=== Moviendo TankBFS hacia (5,5) ===" << endl;
    tanqueBFS.moverse(5, 5, grafo);
    cout << "TankBFS posicion final: (" << tanqueBFS.getX() << ", " << tanqueBFS.getY() << ")" << endl;

    cout << "\n=== Mapa tras movimiento BFS ===" << endl;
    imprimirGrafo(grafo, FILAS, COLS, tanqueBFS.getX(), tanqueBFS.getY(), tanqueDijkstra.getX(), tanqueDijkstra.getY());

    cout << "\n=== Moviendo TankDijkstra hacia (0,5) ===" << endl;
    tanqueDijkstra.moverse(0, 5, grafo);
    cout << "TankDijkstra posicion final: (" << tanqueDijkstra.getX() << ", " << tanqueDijkstra.getY() << ")" << endl;

    cout << "\n=== Mapa final ===" << endl;
    imprimirGrafo(grafo, FILAS, COLS, tanqueBFS.getX(), tanqueBFS.getY(), tanqueDijkstra.getX(), tanqueDijkstra.getY());

    cout << "\n=== Probando dano ===" << endl;
    tanqueBFS.recibirDano(25);
    cout << "Vida TankBFS: " << tanqueBFS.getVida() << endl;
    tanqueDijkstra.recibirDano(50);
    cout << "Vida TankDijkstra: " << tanqueDijkstra.getVida() << endl;
    cout << "TankBFS vivo: " << (tanqueBFS.estaVivo() ? "si" : "no") << endl;
    cout << "TankDijkstra vivo: " << (tanqueDijkstra.estaVivo() ? "si" : "no") << endl;

    return 0;
}