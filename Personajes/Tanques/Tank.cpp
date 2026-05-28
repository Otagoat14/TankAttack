#include <iostream>
#include "Tank.h"
#include "../../Utils.h"
#include "../Balas/Bala.h"

using namespace std;


// Constructor
Tank::Tank(int x, int y, int vida, Equipo equipo, Color color)
    : x(x), y(y), vida(vida), vidaMax(vida), equipo(equipo), color(color)
{
    rutaPendiente   = nullptr; // nullptr evita que apunte a memoria basura
    tiempoAcumulado = 0;
    tiempoMovimiento = 0.15;   // El tanque avanza un paso cada 0.15 segundos reales
    enMovimiento    = false;
    nodoActual      = nullptr;
}

// Disparar

// Crea una Bala en el heap con new y la retorna
// 'this' le pasa al constructor de Bala una referencia al tanque que disparo,
// para que la bala sepa a quien no debe hacerle dano.
// PantallaJuego es responsable de hacer delete cuando la bala ya no este activa.
Bala* Tank::disparar(int destinoX, int destinoY,
                     bool modoAEstrella, bool modoPoder) {
    return new Bala(getX(), getY(), destinoX, destinoY, this,
                    modoAEstrella, modoPoder);
}

// Getters
int Tank::getX() const { return x; }
int Tank::getY() const { return y; }
int Tank::getVida() const { return vida; }
int Tank::getVidaMax() const { return vidaMax; }
Color Tank::getColor() const { return color; }
Equipo Tank::getEquipo() const { return equipo; }
Direccion Tank::getDireccion() { return direccion; }
bool Tank::estaVivo() const { return vida > 0; }


void Tank::recibirDano(int danoRecibido) {
    vida -= danoRecibido;
    cout << "Recibido " << danoRecibido << endl;
}


// Ademas de actualizar (x, y,) calcula automaticamente la nueva direccion
// comparando la posicion nueva con la anterior,
// esto permite que el sprite se dibuje mirando hacia donde se mueve el tanque
void Tank::setPosition(int newX, int newY) {
    if (newX != x) {
        // El tanque se movio horizontalmente
        if (newX < x) direccion = Direccion::ESTE;
        else          direccion = Direccion::OESTE;
    } else {
        // El tanque se movio verticalmente
        if (newY < y) direccion = Direccion::SUR;
        else          direccion = Direccion::NORTE;
    }
    x = newX;
    y = newY;
}

void Tank::setDireccion(Direccion dir) { direccion = dir; }

bool Tank::puedeMoverse(int nx, int ny, const Graph& grafo) {
    return grafo.esPasable(ny, nx);
}


Punto Tank::buscarPosAleatoria(int radio, const Graph& grafo) {
    // Primera pasada: contar candidatos validos
    int count = 0;
    for (int dy = -radio; dy <= radio; dy++) {
        for (int dx = -radio; dx <= radio; dx++) {
            if (dx == 0 && dy == 0) continue; // Saltar la posicion actual del tanque
            if (grafo.esPasable(getY() + dy, getX() + dx))
                count++;
        }
    }

    if (count == 0) return Punto(-1, -1); // No hay celdas disponibles

    // rand() % count genera un numero entre 0 y count-1
    int elegido = rand() % count;
    int actual = 0;

    // Segunda pasada: encontrar la celda en la posicion "elegido"
    for (int dy = -radio; dy <= radio; dy++) {
        for (int dx = -radio; dx <= radio; dx++) {
            if (dx == 0 && dy == 0) continue;
            if (grafo.esPasable(getY() + dy, getX() + dx)) {
                if (actual == elegido)
                    return Punto(getX() + dx, getY() + dy); // Esta es la elegida
                actual++;
            }
        }
    }
    return Punto(-1, -1);
}


// Traza una linea recta al destino usando celdasLineaVista
// y mueve el tanque por esa linea hasta topar con un obstaculo
//Sirve para el movimiento aleatorio
void Tank::avanzarHastaDestino(int destinoX, int destinoY, Graph& grafo) {
    Camino camino = celdаsLineaVista(getX(), getY(), destinoX, destinoY, grafo);

    // Si el camino tiene 1 o menos nodos, el tanque ya esta bloqueado desde el inicio
    if (camino.getTamano() <= 1) {
        cout << "El tanque no pudo avanzar" << endl;
        return;
    }

    // Verificar si el ultimo nodo llego al destino o se detuvo antes
    NodoCamino* actual = camino.getCabeza();
    while (actual->siguiente != nullptr)
        actual = actual->siguiente;

    if (actual->x == destinoX && actual->y == destinoY)
        cout << "Llego al destino" << endl;
    else
        cout << "Avanzando hasta donde sea posible" << endl;

    moverPorCeldas(camino, grafo);
}



//Si hay linea de vista libre al destino: avanza directo
//Si hay un obstaculo: busca un punto intermedio aleatorio cercano,
//avanza hasta ahi, y desde ese punto intenta llegar al destino
void Tank::movimientoAleatorio(int destinoX, int destinoY, Graph& grafo) {
    if (lineaVista(getX(), getY(), destinoX, destinoY, grafo)) {
        cout << "Linea vista libre al destino" << endl;
        avanzarHastaDestino(destinoX, destinoY, grafo);
        return;
    }

    // Sin linea de vista: buscar punto intermedio aleatorio dentro de radio 2
    Punto p = buscarPosAleatoria(2, grafo);
    if (p.x == -1) return; // No hay ninguna celda libre cerca, no se puede mover

    avanzarHastaDestino(p.x, p.y, grafo);           // Avanzar al punto intermedio
    cout << "Segundo intento hacia el destino..." << endl;
    avanzarHastaDestino(destinoX, destinoY, grafo); // Desde ahi intentar llegar al destino
}


// Reconstruye el camino optimo a partir del arreglo padre[] generado por BFS o Dijkstra.
Camino Tank::reconstruirCamino(Graph& grafo, int* padre, int nodoDestino) {
    // Primera pasada: contar longitud del camino
    int longitud = 0;
    int actual   = nodoDestino;
    while (actual != -1) {
        longitud++;
        actual = padre[actual]; // Seguir hacia atras por la cadena de padres
    }

    // Arreglo temporal para invertir el orden (de destino -> inicio / inicio -> destino)
    int* ruta = new int[longitud];
    actual    = nodoDestino;
    for (int i = longitud - 1; i >= 0; i--) {
        ruta[i] = actual;       // Llena de atras hacia adelante
        actual  = padre[actual];
    }

    // Construir Camino con coordenadas en orden correcto
    Camino camino;
    for (int i = 0; i < longitud; i++) {
        int row, col;
        grafo.getCords(ruta[i], row, col); // Convierte indice de nodo a (fila, columna)
        camino.push(col, row);
    }

    delete[] ruta;
    return camino;
}


// Recibe la ruta calculada y prepara el tanque para recorrerla
// El delete rutaPendiente funciona en caso de que si el tanque recibe una ruta nueva antes de
// terminar la anterior, libera la memoria de la vieja para evitar un memory leak.
// Salta el primer nodo porque es la posicion actual del tanque.
void Tank::iniciarMovimiento(Camino* camino)
{
    tiempoAcumulado = 0;
    enMovimiento    = true;

    delete rutaPendiente;  // Liberar ruta anterior si existe
    rutaPendiente = camino;

    nodoActual = camino->getCabeza();
    if (nodoActual != nullptr)
        nodoActual = nodoActual->siguiente; // Saltar la posicion actual

    tiempoAcumulado = 0;
    enMovimiento    = true;
}


// Se llama cada frame desde PantallaJuego con el delta time (dt = segundos desde
// el ultimo frame). Acumula el tiempo y cuando supera tiempoMovimiento (0.15s)
// avanza un paso en la ruta. Esto hace que el movimiento sea igual en cualquier
// maquina sin importar los fps.
void Tank::actualizar(float dt)
{
    // Si no esta en movimiento o ya llego al final de la ruta, no hacer nada
    if (!enMovimiento || nodoActual == nullptr) {
        return;
    }

    tiempoAcumulado += dt; // Acumular tiempo transcurrido
    cout << "Pos: x=" << x << " y=" << y << endl;

    if (tiempoAcumulado >= tiempoMovimiento) {
        setPosition(nodoActual->x, nodoActual->y); // Avanzar al siguiente nodo
        nodoActual      = nodoActual->siguiente;   // Apuntar al proximo paso
        tiempoAcumulado = 0;                       // Resetear el acumulador

        // Si no hay mas nodos, la ruta termino
        if (nodoActual == nullptr) enMovimiento = false;
    }
}


// Recorre el camino nodo por nodo actualizando la posicion del tanque.
// Salta el primer nodo porque es la posicion actual.
void Tank::moverPorCeldas(const Camino& camino, Graph& grafo) {
    NodoCamino* actual = camino.getCabeza();

    if (actual != nullptr)
        actual = actual->siguiente; // Saltar posicion actual

    while (actual != nullptr) {
        setPosition(actual->x, actual->y);
        actual = actual->siguiente;
    }
}

//Destructor
Tank::~Tank()
{
    delete rutaPendiente;
}