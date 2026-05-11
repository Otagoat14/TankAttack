#include "Boton.h"

using namespace std;

Boton::Boton(float x, float y, float ancho, float alto,
             const string& etiqueta,
             function<void()> accion,
             sf::Color colorNormal,
             sf::Color colorHover,
             sf::Color colorBorde,
             sf::Color colorBordeHover,
             sf::Color colorTexto,
             sf::Color colorTextoHover)
    : colorNormal(colorNormal),
      colorHover(colorHover),
      colorBordeNormal(colorBorde),
      colorBordeHover(colorBordeHover),
      colorTextoNormal(colorTexto),
      colorTextoHover(colorTextoHover),
      estaHover(false),
      accion(accion) {

    // Fondo
    fondo.setSize(sf::Vector2f(ancho, alto));
    fondo.setOrigin(ancho / 2.0f, alto / 2.0f);
    fondo.setPosition(x, y);
    fondo.setFillColor(colorNormal);
    fondo.setOutlineColor(colorBorde);
    fondo.setOutlineThickness(2);

    // Brillo decorativo
    brillo.setSize(sf::Vector2f(ancho - 4, 3));
    brillo.setOrigin((ancho - 4) / 2.0f, 0);
    brillo.setPosition(x, y - alto / 2.0f + 4);
    brillo.setFillColor(sf::Color(255, 255, 255, 25));

    // Texto
    texto.setFont(UIManager::instancia().getFuente());
    texto.setString(etiqueta);
    texto.setCharacterSize(16);
    texto.setFillColor(colorTexto);
    centrarTexto();
}

void Boton::centrarTexto() {
    sf::FloatRect fb = fondo.getGlobalBounds();
    sf::FloatRect tb = texto.getLocalBounds();
    texto.setOrigin(tb.left + tb.width / 2.0f,
                    tb.top  + tb.height / 2.0f);
    texto.setPosition(fb.left + fb.width  / 2.0f,
                      fb.top  + fb.height / 2.0f);
}

void Boton::actualizar(sf::Vector2i mousePos) {
    estaHover = fondo.getGlobalBounds().contains(
        static_cast<float>(mousePos.x),
        static_cast<float>(mousePos.y)
    );

    if (estaHover) {
        fondo.setFillColor(colorHover);
        fondo.setOutlineColor(colorBordeHover);
        texto.setFillColor(colorTextoHover);

        // Desplazar texto ligeramente a la derecha
        sf::FloatRect fb = fondo.getGlobalBounds();
        sf::FloatRect tb = texto.getLocalBounds();
        texto.setPosition(fb.left + fb.width / 2.0f + 5,
                          fb.top  + fb.height / 2.0f);
    } else {
        fondo.setFillColor(colorNormal);
        fondo.setOutlineColor(colorBordeNormal);
        texto.setFillColor(colorTextoNormal);
        centrarTexto();
    }
}

void Boton::manejarEvento(sf::Event& evento, sf::Vector2i mousePos) {
    if (evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left &&
        estaHover) {

        UIManager::instancia().reproducirClick();
        if (accion) accion();
    }
}

void Boton::setEtiqueta(const string& etiqueta) {
    texto.setString(etiqueta);
    centrarTexto();
}

void Boton::setAccion(function<void()> nuevaAccion) {
    accion = nuevaAccion;
}

void Boton::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(fondo);
    ventana.draw(brillo);
    ventana.draw(texto);
}