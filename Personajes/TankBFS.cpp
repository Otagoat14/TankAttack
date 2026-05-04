#include <iostream>
#include <vector>
#include "../TankBFS.h"

using namespace std;

tankBFS::tankBFS(int x, int y, int vida, Equipo equipo, Color color)
: Tank(x, y, vida, equipo, color) {}

void tankBFS::moverse(vector<vector<bool>>& matriz){
    int nx, ny;
    cout << "Y: "; cin >> ny;
    cout << "X: "; cin >> nx;

    int posActualX = getX();
    int posActualY = getY();

    if (puedeMoverse(nx, ny, matriz)) {
        matriz[posActualY][posActualX] = false;
        setPosition(nx, ny);
        cout << "El tanque se movio a: (" << nx << ", " << ny << ")" << endl;
    } else {
        cout << "La celda esta fuera del mapa o esta bloqueada" << endl;
    }
}

//Ya la cola fue implementada, queda implementarla con el grafo
void tankBFS::BFS(int inicio, vector<vector<int>>& adj, int numVertices) {
    vector<bool> visitado(numVertices, false);
    Cola<int> cola;

    visitado[inicio] = true;
    cola.enqueue(inicio);

    while (!cola.empty()) {
        int v = cola.getFrente();
        cola.dequeue();

        for (int vecino : adj[v]) {
            if (!visitado[vecino]) {
                visitado[vecino] = true;
                cola.enqueue(vecino);
            }
        }
    }


}