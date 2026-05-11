#include <SFML/Graphics.hpp>
#include "Interfaz/Pantallas/PantallaInicio.h"
#include "Interfaz/Pantallas/PantallaJuego.h"

int main() {
    sf::RenderWindow ventana(
        sf::VideoMode(1280, 720),
        "Tank Attack!",
        sf::Style::Close
    );
    ventana.setFramerateLimit(60);

    bool enJuego  = false;
    bool corriendo = true;

    // PantallaJuego se crea con puntero nulo hasta que el jugador presione Jugar
    PantallaJuego* pantallaJuego = nullptr;

    PantallaInicio* pantalla = new PantallaInicio(
        1280, 720,
        [&]() {
            enJuego = true;
            // Se crea la pantalla de juego solo cuando el jugador presiona Jugar
            // Esto asegura que el mapa se genere fresco cada partida
            delete pantallaJuego;
            pantallaJuego = new PantallaJuego(1280, 720,
                [&]() {
                    // Volver al menú principal
                    enJuego = false;
                }
            );
        },
        [&]() { corriendo = false; }
    );

    sf::Clock reloj;

    while (ventana.isOpen() && corriendo) {
        float dt = reloj.restart().asSeconds();
        sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);

        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();

            if (!enJuego)
                pantalla->manejarEvento(evento, mousePos);
            else if (pantallaJuego != nullptr)
                pantallaJuego->manejarEvento(evento, mousePos);
        }

        ventana.clear();

        if (!enJuego) {
            pantalla->actualizar(dt, mousePos);
            pantalla->dibujar(ventana);
        } else if (pantallaJuego != nullptr) {
            pantallaJuego->actualizar(dt, mousePos);
            pantallaJuego->dibujar(ventana);
        }

        ventana.display();
    }

    delete pantalla;
    delete pantallaJuego;
    return 0;
}