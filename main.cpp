#include <iostream>
#include <vector>
#include "Personajes/TankBFS.h"

using namespace std;

int main() {
    vector<vector<int>> matriz = {
        {0, 0, 0, 0, 0},
        {1, 1, 0, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 0}
    };

    tankBFS tanquePrueba(0, 0, 100, Equipo::JUGADOR1, Color::CELESTE);
    Posicion inicio = {0, 0};
    Posicion final = {4, 4};

    vector<Posicion> camino = tanquePrueba.BFS(matriz, inicio, final);

    if (!camino.empty()) {
        cout << "Camino encontrado:" << endl;
        for (const auto& p : camino) {
            cout << "(" << p.r << ", " << p.c << ") ";
        }
    } else {
        cout << "No hay camino posible." << endl;
    }

    return 0;
}
