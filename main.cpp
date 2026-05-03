#include <iostream>
#include <vector>
#include "Personajes/Tank.h"
#include "Personajes/TankBFS.h"

int main() {
    int filas = 3;
    int columnas = 4;

    vector<vector<bool>> matriz(filas, vector<bool>(columnas, false));

    matriz[0][0] = true;
    matriz[0][1] = true;
    matriz[1][1] = true;
    matriz[1][2] = true;
    matriz[2][2] = true;
    matriz[2][3] = true;

    tankBFS tanquePrueba(0, 0, 100, Equipo::JUGADOR1, Color::CELESTE);

    while (tanquePrueba.getX() != 3 || tanquePrueba.getY() != 2) {
        tanquePrueba.moverse(matriz);
    }

    cout << "\nMapa final:" << endl;
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}