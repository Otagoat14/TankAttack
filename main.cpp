#include <iostream>
#include <cstdlib>
#include <ctime>
#include "str/Grafo.h"
#include "str/Map.h"
#include "Personajes/Tanques/Tank.h"
#include "Personajes/Tanques/TankBFS.h"
#include "Personajes/Tanques/TankDijkstra.h"

using namespace std;

void imprimirMapa(Map& map, Graph& grafo, int filas, int cols,
                  int xBFS, int yBFS, int xDijk, int yDijk) {
    for (int r = 0; r < filas; r++) {
        for (int c = 0; c < cols; c++) {
            int nodo = grafo.getNodo(r, c);
            int peso = map.getPeso(nodo);

            if (r == yBFS && c == xBFS)
                cout << " B ";
            else if (r == yDijk && c == xDijk)
                cout << " D ";
            else if (peso == -1)
                cout << " # ";
            else if (peso == 2)
                cout << " ~ ";  // barro
            else if (peso == 5)
                cout << " W ";  // inundado
            else
                cout << " . ";
        }
        cout << endl;
    }
    cout << "B=TankBFS  D=TankDijkstra  #=obstaculo  ~=barro  W=inundado  .=libre" << endl;
}

int main() {
    srand(time(nullptr));

    const int FILAS = 10;
    const int COLS  = 10;
    const int PORC_OBSTACULOS = 20;  // 20% de obstaculos

    // Crear grafo y mapa
    Graph grafo(FILAS, COLS);
    Map   map(grafo, FILAS, COLS, PORC_OBSTACULOS);
    map.doMapa();

    cout << "=== Mapa generado ===" << endl;

    // Buscar posiciones libres para los tanques
    // TankBFS en esquina superior izquierda
    int xBFS = -1, yBFS = -1;
    for (int r = 0; r < FILAS && xBFS == -1; r++)
        for (int c = 0; c < COLS && xBFS == -1; c++)
            if (map.getPeso(grafo.getNodo(r, c)) != -1) {
                yBFS = r; xBFS = c;
            }

    // TankDijkstra en esquina inferior derecha
    int xDijk = -1, yDijk = -1;
    for (int r = FILAS - 1; r >= 0 && xDijk == -1; r--)
        for (int c = COLS - 1; c >= 0 && xDijk == -1; c--)
            if (map.getPeso(grafo.getNodo(r, c)) != -1) {
                yDijk = r; xDijk = c;
            }

    cout << "TankBFS inicio:      (" << xBFS  << ", " << yBFS  << ")" << endl;
    cout << "TankDijkstra inicio: (" << xDijk << ", " << yDijk << ")" << endl;

    tankBFS      tanqueBFS(xBFS,  yBFS,  100, Equipo::JUGADOR1, Color::AZUL);
    tankDijkstra tanqueDijk(xDijk, yDijk, 100, Equipo::JUGADOR2, Color::ROJO);

    imprimirMapa(map, grafo, FILAS, COLS,
                 tanqueBFS.getX(),  tanqueBFS.getY(),
                 tanqueDijk.getX(), tanqueDijk.getY());

    // =====================
    // PRUEBA BFS
    // =====================
    cout << "\n=== PRUEBA BFS ===" << endl;
    cout << "Moviendo TankBFS hacia TankDijkstra (" << xDijk << ", " << yDijk << ")" << endl;
    tanqueBFS.moverse(xDijk, yDijk, grafo, map);
    cout << "TankBFS posicion final: (" << tanqueBFS.getX() << ", " << tanqueBFS.getY() << ")" << endl;

    imprimirMapa(map, grafo, FILAS, COLS,
                 tanqueBFS.getX(),  tanqueBFS.getY(),
                 tanqueDijk.getX(), tanqueDijk.getY());

    // =====================
    // PRUEBA DIJKSTRA
    // =====================
    cout << "\n=== PRUEBA DIJKSTRA ===" << endl;
    cout << "Moviendo TankDijkstra hacia TankBFS (" << xBFS << ", " << yBFS << ")" << endl;
    tanqueDijk.moverse(xBFS, yBFS, grafo, map);
    cout << "TankDijkstra posicion final: (" << tanqueDijk.getX() << ", " << tanqueDijk.getY() << ")" << endl;

    imprimirMapa(map, grafo, FILAS, COLS,
                 tanqueBFS.getX(),  tanqueBFS.getY(),
                 tanqueDijk.getX(), tanqueDijk.getY());

    // =====================
    // PRUEBA MOVIMIENTO ALEATORIO
    // =====================
    cout << "\n=== PRUEBA MOVIMIENTO ALEATORIO ===" << endl;

    // Buscar destino aleatorio libre para cada tanque
    int xDestBFS = -1, yDestBFS = -1;
    while (xDestBFS == -1) {
        int r = rand() % FILAS;
        int c = rand() % COLS;
        if (map.getPeso(grafo.getNodo(r, c)) != -1) {
            yDestBFS = r; xDestBFS = c;
        }
    }

    int xDestDijk = -1, yDestDijk = -1;
    while (xDestDijk == -1) {
        int r = rand() % FILAS;
        int c = rand() % COLS;
        if (map.getPeso(grafo.getNodo(r, c)) != -1) {
            yDestDijk = r; xDestDijk = c;
        }
    }

    cout << "TankBFS movimiento aleatorio hacia ("
         << xDestBFS << ", " << yDestBFS << ")" << endl;
    tanqueBFS.movimientoAleatorio(xDestBFS, yDestBFS, grafo);
    cout << "TankBFS posicion final: ("
         << tanqueBFS.getX() << ", " << tanqueBFS.getY() << ")" << endl;

    imprimirMapa(map, grafo, FILAS, COLS,
                 tanqueBFS.getX(),  tanqueBFS.getY(),
                 tanqueDijk.getX(), tanqueDijk.getY());

    cout << "\nTankDijkstra movimiento aleatorio hacia ("
         << xDestDijk << ", " << yDestDijk << ")" << endl;
    tanqueDijk.movimientoAleatorio(xDestDijk, yDestDijk, grafo);
    cout << "TankDijkstra posicion final: ("
         << tanqueDijk.getX() << ", " << tanqueDijk.getY() << ")" << endl;

    imprimirMapa(map, grafo, FILAS, COLS,
                 tanqueBFS.getX(),  tanqueBFS.getY(),
                 tanqueDijk.getX(), tanqueDijk.getY());

    // =====================
    // PRUEBA DAÑO
    // =====================
    cout << "\n=== PRUEBA DANO ===" << endl;
    tanqueBFS.recibirDano(25);
    cout << "Vida TankBFS:      " << tanqueBFS.getVida()  << "/100" << endl;
    tanqueDijk.recibirDano(50);
    cout << "Vida TankDijkstra: " << tanqueDijk.getVida() << "/100" << endl;
    cout << "TankBFS vivo:      " << (tanqueBFS.estaVivo()  ? "si" : "no") << endl;
    cout << "TankDijkstra vivo: " << (tanqueDijk.estaVivo() ? "si" : "no") << endl;

    return 0;
}