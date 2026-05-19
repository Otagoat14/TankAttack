#ifndef PANTALLAJUEGO_H
#define PANTALLAJUEGO_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include "../UI/Boton.h"
#include "../UI/Texto.h"
#include "../UI/BarraVida.h"
#include "../UI/UIManager.h"
#include "../../str/MapRender.h"
#include "../../Personajes/Tanques/TankRenderer.h"

using namespace std;

class PantallaJuego {
    int ancho, alto;

    //Manejo de tanques
    int tanqueSeleccionado;
    void manejarClick(int col, int row);

    //Necesarias para el dibujo del mapa
    Graph* grafo;
    Map* mapa;
    MapRender* mapRender;

    //Necesarias para dibujo de tanques
    Tank*         tanques[4];
    TankRenderer* tankRenderer;

    // ── Fondo general ──
    sf::RectangleShape fondoGeneral;

    // ── Panel superior: turno + tiempo ──
    sf::RectangleShape panelSuperior;
    sf::RectangleShape separadorCentro;   // línea vertical que divide J1 | TIEMPO | J2
    sf::Text           textoLabelJ1;      // "JUGADOR 1"
    sf::Text           textoLabelJ2;      // "JUGADOR 2"
    sf::Text           textoTiempo;       // "05:00"
    sf::Text           textoLabelTiempo;  // "TIEMPO"

    // ── Área del mapa (placeholder) ──
    sf::RectangleShape areaMapaFondo;
    sf::RectangleShape bordeMapa;
    sf::Text           textoPlaceholderMapa;

    // ── Panel inferior: tanques + barras de vida ──
    sf::RectangleShape panelInferior;
    sf::RectangleShape separadorInferior;

    // Jugador 1 (izquierda): 2 tanques
    sf::RectangleShape iconoTanqueJ1[2];
    sf::Text           labelTanqueJ1[2];
    BarraVida*         barraVidaJ1[2];

    // Jugador 2 (derecha): 2 tanques
    sf::RectangleShape iconoTanqueJ2[2];
    sf::Text           labelTanqueJ2[2];
    BarraVida*         barraVidaJ2[2];

    // ── Espacios de power-ups ──
    sf::RectangleShape panelPowerUpJ1;
    sf::RectangleShape panelPowerUpJ2;
    sf::Text           labelPowerUpJ1;
    sf::Text           labelPowerUpJ2;

    // ── Botón menú ──
    Boton* botonMenu;

    // ── Construcción ──
    void construirFondo();
    void construirPanelSuperior();
    void construirAreaMapa();
    void construirPanelInferior();
    void construirPanelPowerUps();

    // ── Dibujo ──
    void dibujarPanelSuperior(sf::RenderWindow& v);
    void dibujarAreaMapa(sf::RenderWindow& v);
    void dibujarPanelInferior(sf::RenderWindow& v);
    void dibujarPanelPowerUps(sf::RenderWindow& v);

    //Costruir mapa
    void construirMapa();
    //Dibujar tanques en mapa
    void construirTanques();

public:
    PantallaJuego(int ancho, int alto, function<void()> onMenuPrincipal);
    ~PantallaJuego();

    void manejarEvento(sf::Event& evento, sf::Vector2i mousePos);
    void actualizar(float dt, sf::Vector2i mousePos);
    void dibujar(sf::RenderWindow& ventana);
};

#endif