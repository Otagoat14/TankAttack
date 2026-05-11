#include "Texto.h"

using namespace std;

Texto::Texto(const string& contenido, float x, float y,
             EstiloTexto estilo, bool centrado) {

    texto.setFont(UIManager::instancia().getFuente());
    sombra.setFont(UIManager::instancia().getFuente());
    texto.setString(contenido);
    sombra.setString(contenido);
    tieneSombra = false;

    switch (estilo) {
        case EstiloTexto::TITULO:
            texto.setCharacterSize(36);
            texto.setFillColor(UIColores::AMARILLO_NEON);
            texto.setOutlineColor(sf::Color(180, 140, 0));
            texto.setOutlineThickness(2);
            sombra.setCharacterSize(36);
            sombra.setFillColor(sf::Color(0, 0, 0, 160));
            tieneSombra = true;
            break;
        case EstiloTexto::SUBTITULO:
            texto.setCharacterSize(20);
            texto.setFillColor(UIColores::BLANCO_DESG);
            break;
        case EstiloTexto::NORMAL:
            texto.setCharacterSize(12);
            texto.setFillColor(sf::Color(170, 175, 150));
            break;
        case EstiloTexto::DESTACADO:
            texto.setCharacterSize(18);
            texto.setFillColor(UIColores::VERDE_RADIO);
            break;
        case EstiloTexto::INSTITUCIONAL:
            texto.setCharacterSize(7);
            texto.setFillColor(sf::Color(100, 110, 90));
            break;
    }

    if (centrado) {
        sf::FloatRect b = texto.getLocalBounds();
        texto.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
        sombra.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
    }

    texto.setPosition(x, y);
    sombra.setPosition(x + 3, y + 3);
}

void Texto::setContenido(const string& contenido) {
    texto.setString(contenido);
    sombra.setString(contenido);
}

void Texto::setColor(sf::Color color) {
    texto.setFillColor(color);
}

void Texto::dibujar(sf::RenderWindow& ventana) {
    if (tieneSombra) ventana.draw(sombra);
    ventana.draw(texto);
}