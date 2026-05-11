#ifndef PANTALLAINICIO_H
#define PANTALLAINICIO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <cstdlib>
#include <cmath>
#include "../UI/Boton.h"
#include "../UI/Texto.h"
#include "../UI/UIManager.h"

using namespace std;

// ─── Partícula de chispa ──────────────────────────────────────────────────────
struct Chispa {
    sf::CircleShape forma;
    sf::Vector2f velocidad;
    float vida;        // 0.0 a 1.0
    float vidaMax;
};

// ─── Explosión pequeña ────────────────────────────────────────────────────────
struct Explosion {
    sf::CircleShape anillo;
    sf::CircleShape centro;
    float radio;
    float radioMax;
    float vida;
    sf::Vector2f pos;
};

// ─── Bala cruzando el fondo ───────────────────────────────────────────────────
struct BalaFondo {
    sf::RectangleShape forma;
    sf::RectangleShape estela;
    sf::Vector2f velocidad;
    float x, y;
    bool activa;
};


// ─── Letra del título con daño ───────────────────────────────────────────────
struct LetraTitulo {
    sf::Text texto;
    float    offsetX;      // vibración horizontal
    float    offsetY;      // vibración vertical
    float    tiempoVibra;  // cuánto tiempo le queda vibrando
    int      nivelDanio;   // 0=intacta, 1=dañada, 2=muy dañada
    float    tiempoProxImpacto;  // cuándo recibe el próximo balazo
    sf::Color colorBase;
};

class PantallaInicio {
    // Dimensiones
    int ancho;
    int alto;

    // ── Fondo ──

    sf::Texture textureFondo;
    sf::Sprite  spriteFondo;
    bool        fondoCargado;
    vector<sf::RectangleShape> grietas;

    // ── Elementos del campo de batalla ──
    vector<BalaFondo>   balasFondo;
    vector<Explosion>   explosiones;
    vector<Chispa>      chispas;

    // ── Título con efecto de daño ──
    vector<LetraTitulo> letras;
    sf::Clock           relojTitulo;

    // ── Líneas decorativas ──
    sf::RectangleShape lineaArriba;
    sf::RectangleShape lineaAbajo;

    // ── Botones ──
    Boton* botonJugar;
    Boton* botonSalir;

    // ── Texto institucional ──
    sf::Text textoITCR;
    sf::Text textoInstrucciones;

    // ── Relojes ──
    sf::Clock relojBajas;    // genera nuevas balas en el fondo
    sf::Clock relojExplos;   // genera explosiones aleatorias

    sf::RectangleShape      fondoTitulo;     // rectángulo central

    // ── Construcción ──
    void construirFondo();
    void construirFondoTitulo();
    void construirTitulo();
    void construirTextoExtra();

    // ── Actualización ──
    void actualizarTitulo(float dt);
    void actualizarBalasFondo(float dt);
    void actualizarExplosiones(float dt);
    void actualizarChispas(float dt);
    void generarBalaFondo();
    void generarExplosion(float x, float y);
    void generarChispas(float x, float y, sf::Color color, int cantidad);
    void aplicarImpactoLetra(LetraTitulo& letra);

    // ── Dibujo ──
    void dibujarFondo(sf::RenderWindow& v);
    void dibujarBalasFondo(sf::RenderWindow& v);
    void dibujarExplosiones(sf::RenderWindow& v);
    void dibujarChispas(sf::RenderWindow& v);
    void dibujarTitulo(sf::RenderWindow& v);

public:
    PantallaInicio(int ancho, int alto,
                   function<void()> onJugar,
                   function<void()> onSalir);
    ~PantallaInicio();

    void manejarEvento(sf::Event& evento, sf::Vector2i mousePos);
    void actualizar(float dt, sf::Vector2i mousePos);
    void dibujar(sf::RenderWindow& ventana);
};

#endif