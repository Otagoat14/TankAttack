
#ifndef PANTALLAINICIO_H
#define PANTALLAINICIO_H

#include <SFML/Graphics.hpp>

enum class ResultadoInicio {
    JUGAR,
    SALIR,
    NINGUNO
};

class PantallaInicio {
    sf::Font fuente;

    // Título
    sf::Text titulo;

    // Botones
    sf::RectangleShape fondoJugar;
    sf::Text textoJugar;
    sf::RectangleShape fondoSalir;
    sf::Text textoSalir;

    // Instrucciones
    sf::Text instrucciones;

    // Animación parpadeo del título
    sf::Clock relojParpadeo;
    bool tituloVisible;

    // Colores retro
    sf::Color colorFondo;
    sf::Color colorAcento;
    sf::Color colorTexto;
    sf::Color colorBoton;
    sf::Color colorBotonHover;

    void configurarTitulo();
    void configurarBotones();
    void configurarInstrucciones();
    void actualizarParpadeo();
    bool estaEncima(sf::RectangleShape& forma, sf::Vector2i mousePos);

public:
    PantallaInicio(int anchoVentana, int altoVentana);

    // Retorna JUGAR, SALIR o NINGUNO según el input
    ResultadoInicio manejarEvento(sf::Event& evento, sf::Vector2i mousePos);

    void actualizar();
    void dibujar(sf::RenderWindow& ventana);
};

#endif


