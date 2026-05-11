#ifndef TEXTO_H
#define TEXTO_H

#include <SFML/Graphics.hpp>
#include <string>
#include "UIManager.h"

enum class EstiloTexto {
    TITULO,
    SUBTITULO,
    NORMAL,
    DESTACADO,
    INSTITUCIONAL
};

class Texto {
    sf::Text texto;
    sf::Text sombra;
    bool tieneSombra;

public:
    Texto(const string& contenido,
          float x, float y,
          EstiloTexto estilo = EstiloTexto::NORMAL,
          bool centrado = true);

    void setContenido(const string& contenido);
    void dibujar(sf::RenderWindow& ventana);
    void setColor(sf::Color color);
};

#endif