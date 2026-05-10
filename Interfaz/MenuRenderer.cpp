#include "PantallaInicio.h"
#include <iostream>

using namespace std;

PantallaInicio::PantallaInicio(int anchoVentana, int altoVentana) {

    // Colores estilo retro
    colorFondo        = sf::Color(20, 20, 20);       // negro casi puro
    colorAcento       = sf::Color(0, 255, 0);         // verde retro
    colorTexto        = sf::Color(255, 255, 255);     // blanco
    colorBoton        = sf::Color(40, 40, 40);        // gris oscuro
    colorBotonHover   = sf::Color(0, 180, 0);         // verde más oscuro

    tituloVisible = true;

    if (!fuente.loadFromFile("../Recursos/Fuentes/PressStart2P-Regular.ttf")) {
        cout << "Error cargando fuente" << endl;
    }

    configurarTitulo();
    configurarBotones();
    configurarInstrucciones();
}

void PantallaInicio::configurarTitulo() {
    titulo.setFont(fuente);
    titulo.setString("TANK\nATTACK!");
    titulo.setCharacterSize(40);
    titulo.setFillColor(colorAcento);
    titulo.setOutlineColor(sf::Color(0, 100, 0));
    titulo.setOutlineThickness(3);

    // Centrar horizontalmente
    sf::FloatRect bounds = titulo.getLocalBounds();
    titulo.setOrigin(bounds.width / 2, 0);
    titulo.setPosition(400, 80);  // asume ventana 800px
}

void PantallaInicio::configurarBotones() {
    // Botón JUGAR
    fondoJugar.setSize(sf::Vector2f(300, 60));
    fondoJugar.setFillColor(colorBoton);
    fondoJugar.setOutlineColor(colorAcento);
    fondoJugar.setOutlineThickness(2);
    fondoJugar.setOrigin(150, 30);
    fondoJugar.setPosition(400, 380);

    textoJugar.setFont(fuente);
    textoJugar.setString("JUGAR");
    textoJugar.setCharacterSize(20);
    textoJugar.setFillColor(colorTexto);
    sf::FloatRect boundsJ = textoJugar.getLocalBounds();
    textoJugar.setOrigin(boundsJ.width / 2, boundsJ.height / 2);
    textoJugar.setPosition(400, 378);

    // Botón SALIR
    fondoSalir.setSize(sf::Vector2f(300, 60));
    fondoSalir.setFillColor(colorBoton);
    fondoSalir.setOutlineColor(sf::Color(255, 50, 50));
    fondoSalir.setOutlineThickness(2);
    fondoSalir.setOrigin(150, 30);
    fondoSalir.setPosition(400, 470);

    textoSalir.setFont(fuente);
    textoSalir.setString("SALIR");
    textoSalir.setCharacterSize(20);
    textoSalir.setFillColor(colorTexto);
    sf::FloatRect boundsS = textoSalir.getLocalBounds();
    textoSalir.setOrigin(boundsS.width / 2, boundsS.height / 2);
    textoSalir.setPosition(400, 468);
}

void PantallaInicio::configurarInstrucciones() {
    instrucciones.setFont(fuente);
    instrucciones.setString(
        "CONTROLES:\n\n"
        "Click izquierdo  ->  Mover tanque\n\n"
        "Click derecho    ->  Disparar\n\n"
        "Shift            ->  Usar power-up"
    );
    instrucciones.setCharacterSize(10);
    instrucciones.setFillColor(sf::Color(180, 180, 180));
    instrucciones.setPosition(50, 560);
}

void PantallaInicio::actualizarParpadeo() {
    if (relojParpadeo.getElapsedTime().asSeconds() > 0.6f) {
        tituloVisible = !tituloVisible;
        relojParpadeo.restart();
    }
    titulo.setFillColor(tituloVisible
        ? colorAcento
        : sf::Color::Transparent);
}

bool PantallaInicio::estaEncima(sf::RectangleShape& forma,
                                 sf::Vector2i mousePos) {
    return forma.getGlobalBounds().contains(
        static_cast<float>(mousePos.x),
        static_cast<float>(mousePos.y)
    );
}

ResultadoInicio PantallaInicio::manejarEvento(sf::Event& evento,
                                               sf::Vector2i mousePos) {
    // Hover en botones
    if (estaEncima(fondoJugar, mousePos)) {
        fondoJugar.setFillColor(colorBotonHover);
        textoJugar.setFillColor(sf::Color::Black);
    } else {
        fondoJugar.setFillColor(colorBoton);
        textoJugar.setFillColor(colorTexto);
    }

    if (estaEncima(fondoSalir, mousePos)) {
        fondoSalir.setFillColor(sf::Color(150, 0, 0));
        textoSalir.setFillColor(sf::Color::Black);
    } else {
        fondoSalir.setFillColor(colorBoton);
        textoSalir.setFillColor(colorTexto);
    }

    // Click
    if (evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left) {

        if (estaEncima(fondoJugar, mousePos)) return ResultadoInicio::JUGAR;
        if (estaEncima(fondoSalir, mousePos)) return ResultadoInicio::SALIR;
    }

    return ResultadoInicio::NINGUNO;
}

void PantallaInicio::actualizar() {
    actualizarParpadeo();
}

void PantallaInicio::dibujar(sf::RenderWindow& ventana) {
    ventana.clear(colorFondo);

    // Línea decorativa arriba y abajo del título
    sf::RectangleShape lineaArriba(sf::Vector2f(700, 3));
    lineaArriba.setFillColor(colorAcento);
    lineaArriba.setOrigin(350, 0);
    lineaArriba.setPosition(400, 70);

    sf::RectangleShape lineaAbajo(sf::Vector2f(700, 3));
    lineaAbajo.setFillColor(colorAcento);
    lineaAbajo.setOrigin(350, 0);
    lineaAbajo.setPosition(400, 320);

    ventana.draw(lineaArriba);
    ventana.draw(titulo);
    ventana.draw(lineaAbajo);
    ventana.draw(fondoJugar);
    ventana.draw(textoJugar);
    ventana.draw(fondoSalir);
    ventana.draw(textoSalir);
    ventana.draw(instrucciones);
}