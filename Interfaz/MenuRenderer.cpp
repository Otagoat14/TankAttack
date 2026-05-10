#include "MenuRenderer.h"
#include <iostream>
#include <cmath>

using namespace std;

MenuRenderer::MenuRenderer(int anchoVentana, int altoVentana)
    : ancho(anchoVentana), alto(altoVentana),
      tituloVisible(true), glowIntensidad(0) {

    srand(time(nullptr));

    if (!fuente.loadFromFile("../Recursos/Fuentes/PressStart2P-Regular.ttf")) {
        cout << "Error: no se pudo cargar la fuente" << endl;
    }

    construirCamuflaje();
    construirTitulo();
    construirBotones();
    construirTanqueDecorativo(tanqueRojo,  120, 280, COLOR_ROJO_TANQUE,  true);
    construirTanqueDecorativo(tanqueAzul,  640, 280, COLOR_AZUL_TANQUE, false);
    construirPuntosImpacto();
    construirInstrucciones();
    construirInfoITCR();
}

// ─── Fondo camuflaje ─────────────────────────────────────────────────────────

void MenuRenderer::construirCamuflaje() {
    // Fondo base
    sf::RectangleShape base(sf::Vector2f(ancho, alto));
    base.setFillColor(COLOR_FONDO_BASE);
    patronCamuflaje.push_back(base);

    // Parches de camuflaje aleatorios
    sf::Color colores[] = { COLOR_CAMU_1, COLOR_CAMU_2, COLOR_CAMU_3 };
    for (int i = 0; i < 40; i++) {
        sf::RectangleShape parche;
        float w = 40 + rand() % 120;
        float h = 30 + rand() % 80;
        parche.setSize(sf::Vector2f(w, h));
        parche.setPosition(rand() % ancho, rand() % alto);
        parche.setFillColor(colores[rand() % 3]);
        parche.setRotation(rand() % 30 - 15);
        patronCamuflaje.push_back(parche);
    }
}

// ─── Título ──────────────────────────────────────────────────────────────────

void MenuRenderer::construirTitulo() {
    // Sombra exterior (glow verde)
    tituloSombra1.setFont(fuente);
    tituloSombra1.setString("TANK ATTACK!");
    tituloSombra1.setCharacterSize(38);
    tituloSombra1.setFillColor(sf::Color(0, 180, 0, 80));
    centrarTexto(tituloSombra1, 96);
    tituloSombra1.move(4, 4);

    // Sombra interior (oscura)
    tituloSombra2.setFont(fuente);
    tituloSombra2.setString("TANK ATTACK!");
    tituloSombra2.setCharacterSize(38);
    tituloSombra2.setFillColor(sf::Color(0, 0, 0, 180));
    centrarTexto(tituloSombra2, 96);
    tituloSombra2.move(2, 2);

    // Título principal
    titulo.setFont(fuente);
    titulo.setString("TANK ATTACK!");
    titulo.setCharacterSize(38);
    titulo.setFillColor(COLOR_AMARILLO_NEON);
    titulo.setOutlineColor(sf::Color(180, 140, 0));
    titulo.setOutlineThickness(2);
    centrarTexto(titulo, 96);

    // Bordes metálicos alrededor del título
    sf::FloatRect bounds = titulo.getGlobalBounds();
    float margen = 14;
    float bx = bounds.left  - margen;
    float by = bounds.top   - margen;
    float bw = bounds.width + margen * 2;
    float bh = bounds.height + margen * 2;

    // Arriba
    bordesTitulo[0].setSize(sf::Vector2f(bw, 3));
    bordesTitulo[0].setPosition(bx, by);
    // Abajo
    bordesTitulo[1].setSize(sf::Vector2f(bw, 3));
    bordesTitulo[1].setPosition(bx, by + bh);
    // Izquierda
    bordesTitulo[2].setSize(sf::Vector2f(3, bh));
    bordesTitulo[2].setPosition(bx, by);
    // Derecha
    bordesTitulo[3].setSize(sf::Vector2f(3, bh));
    bordesTitulo[3].setPosition(bx + bw, by);

    for (auto& b : bordesTitulo)
        b.setFillColor(COLOR_METAL_CLARO);
}

// ─── Botones ─────────────────────────────────────────────────────────────────

void MenuRenderer::construirBotones() {
    float centroX = ancho / 2.0f;

    // --- Botón JUGAR ---
    fondoJugar.setSize(sf::Vector2f(320, 58));
    fondoJugar.setOrigin(160, 29);
    fondoJugar.setPosition(centroX, 340);
    fondoJugar.setFillColor(COLOR_METAL_OSCURO);
    fondoJugar.setOutlineColor(COLOR_METAL_CLARO);
    fondoJugar.setOutlineThickness(2);

    brilloJugar.setSize(sf::Vector2f(316, 4));
    brilloJugar.setOrigin(158, 0);
    brilloJugar.setPosition(centroX, 313);
    brilloJugar.setFillColor(sf::Color(180, 190, 160, 60));

    textoJugar.setFont(fuente);
    textoJugar.setString("> JUGAR");
    textoJugar.setCharacterSize(18);
    textoJugar.setFillColor(COLOR_BLANCO_DESG);
    centrarTexto(textoJugar, 328);

    // --- Botón SALIR ---
    fondoSalir.setSize(sf::Vector2f(320, 58));
    fondoSalir.setOrigin(160, 29);
    fondoSalir.setPosition(centroX, 420);
    fondoSalir.setFillColor(COLOR_METAL_OSCURO);
    fondoSalir.setOutlineColor(COLOR_METAL_CLARO);
    fondoSalir.setOutlineThickness(2);

    brilloSalir.setSize(sf::Vector2f(316, 4));
    brilloSalir.setOrigin(158, 0);
    brilloSalir.setPosition(centroX, 393);
    brilloSalir.setFillColor(sf::Color(180, 190, 160, 60));

    textoSalir.setFont(fuente);
    textoSalir.setString("> SALIR");
    textoSalir.setCharacterSize(18);
    textoSalir.setFillColor(COLOR_BLANCO_DESG);
    centrarTexto(textoSalir, 408);
}

// ─── Tanques decorativos ─────────────────────────────────────────────────────

void MenuRenderer::construirTanqueDecorativo(TanqueDecorativo& t,
                                              float x, float y,
                                              sf::Color color,
                                              bool mirandoDerecha) {
    t.color = color;
    sf::Color colorOscuro(color.r * 0.6f, color.g * 0.6f, color.b * 0.6f);

    // Cuerpo principal
    t.cuerpo.setSize(sf::Vector2f(80, 40));
    t.cuerpo.setOrigin(40, 20);
    t.cuerpo.setPosition(x, y);
    t.cuerpo.setFillColor(color);
    t.cuerpo.setOutlineColor(colorOscuro);
    t.cuerpo.setOutlineThickness(2);

    // Cañón
    float canonX = mirandoDerecha ? x + 35 : x - 35;
    t.canon.setSize(sf::Vector2f(40, 10));
    t.canon.setOrigin(mirandoDerecha ? 0 : 40, 5);
    t.canon.setPosition(canonX, y - 5);
    t.canon.setFillColor(colorOscuro);

    // Ruedas
    float ruedaY = y + 18;
    t.rueda1.setRadius(10);
    t.rueda1.setOrigin(10, 10);
    t.rueda1.setPosition(x - 28, ruedaY);
    t.rueda1.setFillColor(colorOscuro);

    t.rueda2.setRadius(10);
    t.rueda2.setOrigin(10, 10);
    t.rueda2.setPosition(x, ruedaY);
    t.rueda2.setFillColor(colorOscuro);

    t.rueda3.setRadius(10);
    t.rueda3.setOrigin(10, 10);
    t.rueda3.setPosition(x + 28, ruedaY);
    t.rueda3.setFillColor(colorOscuro);
}

// ─── Puntos de impacto ───────────────────────────────────────────────────────

void MenuRenderer::construirPuntosImpacto() {
    for (int i = 0; i < 25; i++) {
        PuntoImpacto p;
        float radio = 2 + rand() % 5;
        p.estrella.setRadius(radio);
        p.estrella.setOrigin(radio, radio);
        p.estrella.setPosition(rand() % ancho, rand() % alto);
        p.estrella.setFillColor(sf::Color(255, 220, 0, 120 + rand() % 135));
        p.opacidad = 120 + rand() % 135;
        puntosImpacto.push_back(p);
    }
}

// ─── Instrucciones ───────────────────────────────────────────────────────────

void MenuRenderer::construirInstrucciones() {
    instrucciones.setFont(fuente);
    instrucciones.setString(
        "CLICK IZQ: Mover     CLICK DER: Disparar     SHIFT: Power-up"
    );
    instrucciones.setCharacterSize(8);
    instrucciones.setFillColor(sf::Color(150, 160, 130));
    centrarTexto(instrucciones, 510);
}

// ─── Info ITCR ───────────────────────────────────────────────────────────────

void MenuRenderer::construirInfoITCR() {
    infoITCR.setFont(fuente);
    infoITCR.setString("ITCR - Ingenieria en Computadores");
    infoITCR.setCharacterSize(7);
    infoITCR.setFillColor(sf::Color(100, 110, 90));

    sf::FloatRect b = infoITCR.getLocalBounds();
    infoITCR.setPosition(ancho - b.width - 10, alto - b.height - 14);
}

// ─── Actualización ───────────────────────────────────────────────────────────

void MenuRenderer::actualizarParpadeo() {
    if (relojParpadeo.getElapsedTime().asSeconds() > 0.55f) {
        tituloVisible = !tituloVisible;
        relojParpadeo.restart();
    }

    // Efecto glow pulsante en el título
    float t = relojGlow.getElapsedTime().asSeconds();
    glowIntensidad = (sin(t * 3.0f) + 1.0f) / 2.0f;  // 0.0 a 1.0

    sf::Uint8 alpha = static_cast<sf::Uint8>(40 + glowIntensidad * 120);
    tituloSombra1.setFillColor(sf::Color(0, 255, 80, alpha));

    if (tituloVisible)
        titulo.setFillColor(COLOR_AMARILLO_NEON);
    else
        titulo.setFillColor(sf::Color::Transparent);
}

void MenuRenderer::actualizarHover(sf::Vector2i mousePos) {
    // Hover JUGAR
    if (estaEncima(fondoJugar, mousePos)) {
        fondoJugar.setFillColor(sf::Color(0, 80, 20));
        fondoJugar.setOutlineColor(COLOR_VERDE_RADIO);
        textoJugar.setFillColor(COLOR_VERDE_RADIO);
        textoJugar.setPosition(
            ancho / 2.0f - textoJugar.getLocalBounds().width / 2.0f + 6,
            textoJugar.getPosition().y
        );
    } else {
        fondoJugar.setFillColor(COLOR_METAL_OSCURO);
        fondoJugar.setOutlineColor(COLOR_METAL_CLARO);
        textoJugar.setFillColor(COLOR_BLANCO_DESG);
        centrarTexto(textoJugar, 328);
    }

    // Hover SALIR
    if (estaEncima(fondoSalir, mousePos)) {
        fondoSalir.setFillColor(sf::Color(80, 10, 10));
        fondoSalir.setOutlineColor(sf::Color(255, 60, 60));
        textoSalir.setFillColor(sf::Color(255, 80, 80));
        textoSalir.setPosition(
            ancho / 2.0f - textoSalir.getLocalBounds().width / 2.0f + 6,
            textoSalir.getPosition().y
        );
    } else {
        fondoSalir.setFillColor(COLOR_METAL_OSCURO);
        fondoSalir.setOutlineColor(COLOR_METAL_CLARO);
        textoSalir.setFillColor(COLOR_BLANCO_DESG);
        centrarTexto(textoSalir, 408);
    }
}

void MenuRenderer::actualizar() {
    actualizarParpadeo();
}

// ─── Eventos ─────────────────────────────────────────────────────────────────

ResultadoMenu MenuRenderer::manejarEvento(sf::Event& evento,
                                           sf::Vector2i mousePos) {
    actualizarHover(mousePos);

    if (evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left) {
        if (estaEncima(fondoJugar, mousePos)) return ResultadoMenu::JUGAR;
        if (estaEncima(fondoSalir, mousePos)) return ResultadoMenu::SALIR;
    }
    return ResultadoMenu::NINGUNO;
}

// ─── Dibujo ──────────────────────────────────────────────────────────────────

void MenuRenderer::dibujarTanque(sf::RenderWindow& ventana,
                                  TanqueDecorativo& t) {
    ventana.draw(t.rueda1);
    ventana.draw(t.rueda2);
    ventana.draw(t.rueda3);
    ventana.draw(t.cuerpo);
    ventana.draw(t.canon);
}

void MenuRenderer::dibujar(sf::RenderWindow& ventana) {
    // 1. Fondo camuflaje
    for (auto& p : patronCamuflaje)
        ventana.draw(p);

    // 2. Puntos de impacto
    for (auto& p : puntosImpacto)
        ventana.draw(p.estrella);

    // 3. Tanques decorativos
    dibujarTanque(ventana, tanqueRojo);
    dibujarTanque(ventana, tanqueAzul);

    // 4. Bordes metálicos del título
    for (auto& b : bordesTitulo)
        ventana.draw(b);

    // 5. Título con sombras y parpadeo
    ventana.draw(tituloSombra1);
    ventana.draw(tituloSombra2);
    ventana.draw(titulo);

    // 6. Línea separadora
    sf::RectangleShape linea(sf::Vector2f(500, 2));
    linea.setOrigin(250, 1);
    linea.setPosition(ancho / 2.0f, 290);
    linea.setFillColor(sf::Color(120, 130, 100, 160));
    ventana.draw(linea);

    // 7. Botones
    ventana.draw(fondoJugar);
    ventana.draw(brilloJugar);
    ventana.draw(textoJugar);
    ventana.draw(fondoSalir);
    ventana.draw(brilloSalir);
    ventana.draw(textoSalir);

    // 8. Instrucciones e info
    ventana.draw(instrucciones);
    ventana.draw(infoITCR);
}

// ─── Helpers ─────────────────────────────────────────────────────────────────

bool MenuRenderer::estaEncima(sf::RectangleShape& forma,
                               sf::Vector2i mousePos) {
    return forma.getGlobalBounds().contains(
        static_cast<float>(mousePos.x),
        static_cast<float>(mousePos.y)
    );
}

void MenuRenderer::centrarTexto(sf::Text& texto, float y) {
    sf::FloatRect b = texto.getLocalBounds();
    texto.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
    texto.setPosition(ancho / 2.0f, y);
}