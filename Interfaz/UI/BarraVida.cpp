#include "BarraVida.h"

using namespace std;

BarraVida::BarraVida(float x, float y, float ancho, float alto,
                     int vidaMaxima, const string& nombreTanque)
    : anchoMax(ancho), alto(alto), vidaMax(vidaMaxima) {

    // Fondo gris oscuro
    fondo.setSize(sf::Vector2f(ancho, alto));
    fondo.setPosition(x, y);
    fondo.setFillColor(sf::Color(30, 30, 30));
    fondo.setOutlineColor(UIColores::METAL_CLARO);
    fondo.setOutlineThickness(1);

    // Relleno verde
    relleno.setSize(sf::Vector2f(ancho, alto));
    relleno.setPosition(x, y);
    relleno.setFillColor(UIColores::VERDE_RADIO);

    // Etiqueta
    etiqueta.setFont(UIManager::instancia().getFuente());
    etiqueta.setString(nombreTanque);
    etiqueta.setCharacterSize(8);
    etiqueta.setFillColor(UIColores::BLANCO_DESG);
    etiqueta.setPosition(x, y - 14);
}

void BarraVida::actualizar(int vidaActual) {
    float porcentaje = static_cast<float>(vidaActual) / vidaMax;
    porcentaje = max(0.0f, min(1.0f, porcentaje));

    relleno.setSize(sf::Vector2f(anchoMax * porcentaje, alto));

    // Color cambia según vida
    if (porcentaje > 0.6f)
        relleno.setFillColor(UIColores::VERDE_RADIO);
    else if (porcentaje > 0.3f)
        relleno.setFillColor(UIColores::AMARILLO_NEON);
    else
        relleno.setFillColor(UIColores::ROJO);
}

void BarraVida::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(fondo);
    ventana.draw(relleno);
    ventana.draw(etiqueta);
}