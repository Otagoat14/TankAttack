#include <iostream>
#include <vector>
#include <queue>

using namespace std;

enum class Color { AZUL, CELESTE, ROJO, AMARILLO };
enum class Equipo { JUGADOR1, JUGADOR2 };

string colorATexto(Color color) {
    switch(color) {
        case Color::AZUL:
            return "Azul";
        case Color::CELESTE:
            return "Celeste";
        case Color::ROJO:
            return "Rojo";
        case Color::AMARILLO:
            return "Amarillo";
        default:
            return "Desconocido";
    }
}

class Tank {
    int x;
    int y;
    int vida;
    const Equipo equipo;
    const Color color;

public:
    Tank(int x, int y, int vida, Equipo equipo, Color color)
    : x(x), y(y), vida(vida), equipo(equipo), color(color) {}

    virtual void moverse(vector<vector<bool>>& matriz) {
        cout << "Moviendose" << endl;
    }

    void disparar() {
        cout << "Disparar" << endl;
    }
    int getX() const {
        return x;
    }
    int getY() const {
        return y;
    }
    int getVida() const {
        return vida;
    }
    Color getColor() const {
        return color;
    }
    Equipo getEquipo() const {
        return equipo;
    }
    bool estaVivo() const {
        return vida > 0;
    }

    void recibirDano(int danoRecibido) {
        vida -= danoRecibido;
        cout << "Recibido " << danoRecibido << endl;
    }

    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }

    bool puedeMoverse(int nx, int ny, const vector<vector<bool>>& matriz) {
        int filas = matriz.size();
        int columnas = matriz[0].size();

        if (nx >= 0 && ny >= 0 && nx < columnas && ny < filas) {
            return matriz[ny][nx] == true;
        }
        return false;
    }

    virtual ~Tank() = default;
};

class tankBFS : public Tank {
public:
    tankBFS(int x, int y, int vida, Equipo equipo, Color color)
    : Tank(x, y, vida, equipo, color) {}

    void moverse(vector<vector<bool>>& matriz) override {
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

    //Funcion con vector y cola, hay que implementar la cola propia y en vector iria el grafo
    void BFS(int inicio, vector<vector<int>>& adj, int numVertices) {
        vector<bool> visitado(numVertices, false);
        queue<int> cola;

        visitado[inicio] = true;
        cola.push(inicio);

        while (!cola.empty()) {
            int v = cola.front();
            cola.pop();

            for (int vecino : adj[v]) {
                if (!visitado[vecino]) {
                    visitado[vecino] = true;
                    cola.push(vecino);
                }
            }
        }


    }
};

class tankDijkstra : public Tank {
public:
    tankDijkstra(int x, int y, int vida, Equipo equipo, Color color)
    : Tank(x, y, vida, equipo, color) {}

    void moverse(vector<vector<bool>>& matriz) override {
        // Por implementar
    }
};


//Prueba con matriz booleana, mover el tanque
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