#include <SFML/Graphics.hpp>
#include "Interfaz/MenuRenderer.h"

int main() {
    sf::RenderWindow ventana(
        sf::VideoMode(800, 600),
        "Tank Attack!",
        sf::Style::Close
    );
    ventana.setFramerateLimit(60);

    MenuRenderer menu(800, 600);
    bool enMenu = true;

    while (ventana.isOpen() && enMenu) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);

        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();

            ResultadoMenu resultado = menu.manejarEvento(evento, mousePos);
            if (resultado == ResultadoMenu::JUGAR) enMenu = false;
            if (resultado == ResultadoMenu::SALIR) ventana.close();
        }

        menu.actualizar();
        menu.dibujar(ventana);
        ventana.display();
    }

    // Aquí irá el loop del juego
    // while (ventana.isOpen()) { ... }

    return 0;
}