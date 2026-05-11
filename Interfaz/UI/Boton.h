#ifndef BOTON_H
#define BOTON_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include "UIManager.h"

using namespace std;

class Boton {
    sf::RectangleShape fondo;
    sf::RectangleShape brillo;
    sf::Text texto;


    sf::Color colorNormal;
    sf::Color colorHover;
    sf::Color colorBordeNormal;
    sf::Color colorBordeHover;
    sf::Color colorTextoNormal;
    sf::Color colorTextoHover;

    bool estaHover;
    function<void()> accion;

    void centrarTexto();

public:
    Boton(float x, float y, float ancho, float alto,
          const string& etiqueta,
          function<void()> accion,
          sf::Color colorNormal     = UIColores::METAL_OSCURO,
          sf::Color colorHover      = sf::Color(0, 80, 20),
          sf::Color colorBorde      = UIColores::METAL_CLARO,
          sf::Color colorBordeHover = UIColores::VERDE_RADIO,
          sf::Color colorTexto      = UIColores::BLANCO_DESG,
          sf::Color colorTextoHover = UIColores::VERDE_RADIO);

    void manejarEvento(sf::Event& evento, sf::Vector2i mousePos);
    void actualizar(sf::Vector2i mousePos);
    void dibujar(sf::RenderWindow& ventana);

    void setEtiqueta(const string& etiqueta);
    void setAccion(function<void()> nuevaAccion);
    bool getEstaHover() const { return estaHover; }
};

#endif