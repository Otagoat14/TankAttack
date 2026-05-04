#include <iostream>
#include <vector>
#include "TankBFS.h"
#include <algorithm>

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
vector<Posicion> tankBFS::BFS(vector<vector<int>>& matriz, Posicion inicio, Posicion final) {

    //Direcciones en x y
    int dy[] = {-1, 1, 0, 0};
    int dx[] = {0, 0, -1, 1};

    int filas = matriz.size();
    int columnas = matriz[0].size();

    //Celdas visitadas
    vector<vector<bool>> visitado(filas, vector<bool>(columnas, false));
    //Reconstruir el camino
    vector<vector<Posicion>> padre(filas, vector<Posicion>(columnas, {-1, -1}));

    Cola<Posicion> cola;
    cola.enqueue(inicio);
    visitado[inicio.r][inicio.c] = true;

    while (!cola.empty()) {
        Posicion actual = cola.getFrente();
        cola.dequeue();

        //Llegar al objetivo
        if (actual.r == final.r && actual.c == final.c) {
            vector<Posicion> camino;
            for (Posicion p = final; p.r != -1; p = padre[p.r][p.c]) {
                camino.push_back(p);
            }
            reverse(camino.begin(), camino.end());
            return camino;
        }

        //Ver a los 4 vecinos
        for (int i = 0; i < 4; i++) {
            int nr = actual.r + dy[i];
            int nc = actual.c + dx[i];

            //Ver si esta bloqueado o fuera del mapa
            if (nr >= 0 && nr < filas && nc >= 0 && nc < columnas &&
                matriz[nr][nc] == 0 && !visitado[nr][nc]) {

                visitado[nr][nc] = true;
                padre[nr][nc] = actual;
                cola.enqueue({nr, nc});
                }
        }
    }
    //Retorna vacio ni no hay ningun camino
    return {};
}