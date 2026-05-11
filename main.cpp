#include <SFML/Graphics.hpp>
#include "Interfaz/Pantallas/PantallaInicio.h"

int main() {
    sf::RenderWindow ventana(
        sf::VideoMode(1280, 720),
        "Tank Attack!",
        sf::Style::Close
    );
    ventana.setFramerateLimit(60);

    bool enJuego = false;
    bool corriendo = true;

    PantallaInicio* pantalla = new PantallaInicio(
        1280, 720,
        [&]() { enJuego   = true;  },   // onJugar
        [&]() { corriendo = false; }    // onSalir
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
        }

        ventana.clear();

        if (!enJuego) {
            pantalla->actualizar(dt, mousePos);
            pantalla->dibujar(ventana);
        } else {
            // Aquí irá la pantalla del juego
        }

        ventana.display();
    }

    delete pantalla;
    return 0;
}