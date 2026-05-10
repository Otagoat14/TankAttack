#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow ventana(
        sf::VideoMode(800, 600),
        "Tank Attack - Prueba SFML"
    );

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();
        }

        ventana.clear(sf::Color(50, 50, 50));
        ventana.display();
    }
    return 0;
}