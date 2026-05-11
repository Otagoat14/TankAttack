#ifndef BARRAVIDA_H
#define BARRAVIDA_H

#include <SFML/Graphics.hpp>
#include "UIManager.h"

class BarraVida {
    sf::RectangleShape fondo;
    sf::RectangleShape relleno;
    sf::RectangleShape borde;
    sf::Text etiqueta;

    float anchoMax;
    float alto;
    int vidaMax;

public:
    BarraVida(float x, float y, float ancho, float alto,
              int vidaMax, const string& nombreTanque);

    void actualizar(int vidaActual);
    void dibujar(sf::RenderWindow& ventana);
};

#endif