#include "PantallaInicio.h"
#include <iostream>
#include <ctime>

using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────────────────
PantallaInicio::PantallaInicio(int ancho, int alto,
                                function<void()> onJugar,
                                function<void()> onSalir)
    : ancho(ancho), alto(alto) {

    srand(time(nullptr));
    construirFondo();
    construirTitulo();
    construirLineas();
    construirTextoExtra();

    botonJugar = new Boton(
    ancho / 2.0f, alto * 0.62f, 340, 58,
    "> JUGAR",
    onJugar,
    UIColores::PANEL_OSCURO,          // fondo normal
    UIColores::HOVER_JUGAR,           // fondo hover
    UIColores::METAL_CLARO,           // borde normal
    UIColores::VERDE_CLARO,           // borde hover
    UIColores::BLANCO_DESG,           // texto normal
    UIColores::VERDE_CLARO            // texto hover
);

    botonSalir = new Boton(
        ancho / 2.0f, alto * 0.75f, 340, 58,
        "> SALIR",
        onSalir,
        UIColores::PANEL_OSCURO,          // fondo normal
        UIColores::HOVER_SALIR,           // fondo hover
        UIColores::METAL_CLARO,           // borde normal
        UIColores::ACENTO_ROJO,           // borde hover
        UIColores::BLANCO_DESG,           // texto normal
        UIColores::ACENTO_ROJO            // texto hover
    );

    // Generar algunas balas y explosiones iniciales
    for (int i = 0; i < 5; i++) generarBalaFondo();
    for (int i = 0; i < 3; i++)
        generarExplosion(rand() % ancho, rand() % alto);
}

PantallaInicio::~PantallaInicio() {
    delete botonJugar;
    delete botonSalir;
}

// ─────────────────────────────────────────────────────────────────────────────
// Construcción
// ─────────────────────────────────────────────────────────────────────────────

void PantallaInicio::construirFondo() {
    fondoCargado = textureFondo.loadFromFile(
        "../Recursos/Imagenes/fondo_menu.png"
    );

    if (!fondoCargado) {
        cout << "Error: no se pudo cargar el fondo" << endl;
        return;
    }

    spriteFondo.setTexture(textureFondo);

    // Escalar para que cubra toda la ventana
    sf::Vector2u tamTextura = textureFondo.getSize();
    float escalaX = static_cast<float>(ancho) / tamTextura.x;
    float escalaY = static_cast<float>(alto)  / tamTextura.y;
    spriteFondo.setScale(escalaX, escalaY);
}

void PantallaInicio::construirTitulo() {
    string tituloStr = "TANK ATTACK!";
    float  charSize  = 52;
    float  espaciado = charSize * 1.1f;

    // Calcular ancho total para centrar
    float anchoTotal = tituloStr.size() * espaciado * 0.75f;
    float startX     = ancho / 2.0f - anchoTotal / 2.0f;
    float posY       = alto * 0.22f;

    for (int i = 0; i < (int)tituloStr.size(); i++) {
        LetraTitulo l;
        l.texto.setFont(UIManager::instancia().getFuente());
        l.texto.setString(string(1, tituloStr[i]));
        l.texto.setCharacterSize(charSize);
        l.texto.setFillColor(UIColores::AMARILLO_NEON);
        l.texto.setOutlineColor(sf::Color(100, 60, 15));
        l.texto.setOutlineThickness(4);
        l.texto.setPosition(startX + i * espaciado * 0.75f, posY);

        l.offsetX          = 0;
        l.offsetY          = 0;
        l.tiempoVibra      = 0;
        l.nivelDanio       = 0;
        l.colorBase        = UIColores::AMARILLO_NEON;
        l.tiempoProxImpacto = 0.5f + (rand() % 300) / 100.0f;

        letras.push_back(l);
    }
}

void PantallaInicio::construirLineas() {
    float cy = alto * 0.38f;

    lineaArriba.setSize(sf::Vector2f(900, 3));
    lineaArriba.setOrigin(450, 1);
    lineaArriba.setPosition(ancho / 2.0f, cy - 30);
    lineaArriba.setFillColor(sf::Color(100, 120, 60, 180));

    lineaAbajo.setSize(sf::Vector2f(900, 3));
    lineaAbajo.setOrigin(450, 1);
    lineaAbajo.setPosition(ancho / 2.0f, cy + 10);
    lineaAbajo.setFillColor(sf::Color(100, 120, 60, 180));
}


void PantallaInicio::construirTextoExtra() {
    textoITCR.setFont(UIManager::instancia().getFuente());
    textoITCR.setString("ITCR - Ingenieria en Computadores");
    textoITCR.setCharacterSize(8);
    textoITCR.setFillColor(sf::Color(90, 100, 75));
    sf::FloatRect b = textoITCR.getLocalBounds();
    textoITCR.setPosition(ancho - b.width - 12, alto - b.height - 16);

    textoInstrucciones.setFont(UIManager::instancia().getFuente());
    textoInstrucciones.setString(
        "CLICK IZQ: Mover     CLICK DER: Disparar     SHIFT: Power-up"
    );
    textoInstrucciones.setCharacterSize(9);
    textoInstrucciones.setFillColor(sf::Color(120, 130, 90, 180));
    sf::FloatRect bi = textoInstrucciones.getLocalBounds();
    textoInstrucciones.setOrigin(bi.width / 2.0f, 0);
    textoInstrucciones.setPosition(ancho / 2.0f, alto - 38);
}

// ─────────────────────────────────────────────────────────────────────────────
// Generadores
// ─────────────────────────────────────────────────────────────────────────────

void PantallaInicio::generarBalaFondo() {
    BalaFondo b;

    // Sale desde un borde aleatorio
    int borde = rand() % 4;
    float vx = 0, vy = 0;

    if (borde == 0) {        // desde arriba
        b.x = rand() % ancho;
        b.y = -10;
        vx  = (rand() % 200 - 100) * 1.5f;
        vy  = 200 + rand() % 200;
    } else if (borde == 1) { // desde abajo
        b.x = rand() % ancho;
        b.y = alto + 10;
        vx  = (rand() % 200 - 100) * 1.5f;
        vy  = -(200 + rand() % 200);
    } else if (borde == 2) { // desde izquierda
        b.x = -10;
        b.y = rand() % alto;
        vx  = 250 + rand() % 200;
        vy  = (rand() % 200 - 100) * 1.5f;
    } else {                 // desde derecha
        b.x = ancho + 10;
        b.y = rand() % alto;
        vx  = -(250 + rand() % 200);
        vy  = (rand() % 200 - 100) * 1.5f;
    }

    b.velocidad = sf::Vector2f(vx, vy);
    b.activa    = true;

    // Calcular ángulo para orientar la bala
    float angulo = atan2(vy, vx) * 180.0f / 3.14159f;

    b.forma.setSize(sf::Vector2f(18, 5));
    b.forma.setOrigin(9, 2.5f);
    b.forma.setPosition(b.x, b.y);
    b.forma.setFillColor(sf::Color(255, 220, 80));
    b.forma.setRotation(angulo);

    b.estela.setSize(sf::Vector2f(30, 3));
    b.estela.setOrigin(30, 1.5f);
    b.estela.setPosition(b.x, b.y);
    b.estela.setFillColor(sf::Color(255, 150, 30, 80));
    b.estela.setRotation(angulo);

    balasFondo.push_back(b);
}

void PantallaInicio::generarExplosion(float x, float y) {
    Explosion e;
    e.pos      = sf::Vector2f(x, y);
    e.radio    = 2;
    e.radioMax = 20 + rand() % 30;
    e.vida     = 1.0f;

    e.anillo.setRadius(e.radio);
    e.anillo.setOrigin(e.radio, e.radio);
    e.anillo.setPosition(x, y);
    e.anillo.setFillColor(sf::Color::Transparent);
    e.anillo.setOutlineColor(sf::Color(255, 150, 0));
    e.anillo.setOutlineThickness(3);

    e.centro.setRadius(e.radio * 0.5f);
    e.centro.setOrigin(e.radio * 0.5f, e.radio * 0.5f);
    e.centro.setPosition(x, y);
    e.centro.setFillColor(sf::Color(255, 220, 100));

    explosiones.push_back(e);
    generarChispas(x, y, sf::Color(255, 180, 50), 8 + rand() % 8);
}

void PantallaInicio::generarChispas(float x, float y,
                                     sf::Color color, int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        Chispa c;
        float angulo = (rand() % 360) * 3.14159f / 180.0f;
        float speed  = 60 + rand() % 140;

        c.velocidad = sf::Vector2f(cos(angulo) * speed, sin(angulo) * speed);
        c.vida      = 1.0f;
        c.vidaMax   = 0.4f + (rand() % 60) / 100.0f;

        float radio = 2 + rand() % 3;
        c.forma.setRadius(radio);
        c.forma.setOrigin(radio, radio);
        c.forma.setPosition(x, y);
        c.forma.setFillColor(color);

        chispas.push_back(c);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Actualización
// ─────────────────────────────────────────────────────────────────────────────

void PantallaInicio::aplicarImpactoLetra(LetraTitulo& l) {
    l.tiempoVibra = 0.25f;
    l.offsetX     = (rand() % 10 - 5);
    l.offsetY     = (rand() % 8  - 4);
    l.nivelDanio  = min(2, l.nivelDanio + 1);

    // Color según daño
    if (l.nivelDanio == 1)
        l.colorBase = sf::Color(255, 140, 0);   // naranja dañado
    else
        l.colorBase = sf::Color(220, 60, 60);   // rojo muy dañado

    l.texto.setFillColor(l.colorBase);

    // Generar chispas en la posición de la letra
    sf::Vector2f pos = l.texto.getPosition();
    generarChispas(pos.x + 20, pos.y + 20,
                   sf::Color(255, 200, 50), 5 + rand() % 5);
}

void PantallaInicio::actualizarTitulo(float dt) {
    for (auto& l : letras) {
        // Cuenta regresiva para próximo impacto
        l.tiempoProxImpacto -= dt;
        if (l.tiempoProxImpacto <= 0) {
            aplicarImpactoLetra(l);
            // Próximo impacto entre 1 y 4 segundos
            l.tiempoProxImpacto = 1.0f + (rand() % 300) / 100.0f;
        }

        // Vibración
        if (l.tiempoVibra > 0) {
            l.tiempoVibra -= dt;
            l.offsetX = (rand() % 8 - 4) * (l.tiempoVibra / 0.25f);
            l.offsetY = (rand() % 6 - 3) * (l.tiempoVibra / 0.25f);

            if (l.tiempoVibra <= 0) {
                l.offsetX = 0;
                l.offsetY = 0;
            }
        }

        // Aplicar posición con offset
        sf::Vector2f posBase = l.texto.getPosition();
        l.texto.setPosition(posBase.x + l.offsetX,
                            posBase.y + l.offsetY);

        // Parpadeo rápido al recibir daño
        if (l.tiempoVibra > 0.1f) {
            bool visible = (int)(l.tiempoVibra * 20) % 2 == 0;
            l.texto.setFillColor(visible
                ? l.colorBase
                : sf::Color::Transparent);
        } else {
            l.texto.setFillColor(l.colorBase);
        }

        // Restaurar posición base después del offset
        l.texto.setPosition(posBase);
    }
}

void PantallaInicio::actualizarBalasFondo(float dt) {
    for (auto& b : balasFondo) {
        if (!b.activa) continue;

        b.x += b.velocidad.x * dt;
        b.y += b.velocidad.y * dt;

        b.forma.setPosition(b.x, b.y);
        b.estela.setPosition(b.x, b.y);

        // Si sale de pantalla, generar explosión y desactivar
        if (b.x < -20 || b.x > ancho + 20 ||
            b.y < -20 || b.y > alto  + 20) {
            b.activa = false;
        }
    }

    // Eliminar balas inactivas
    for (int i = balasFondo.size() - 1; i >= 0; i--)
        if (!balasFondo[i].activa) balasFondo.erase(balasFondo.begin() + i);

    // Generar nueva bala cada cierto tiempo
    if (relojBajas.getElapsedTime().asSeconds() > 0.8f) {
        generarBalaFondo();
        relojBajas.restart();
    }
}

void PantallaInicio::actualizarExplosiones(float dt) {
    for (auto& e : explosiones) {
        e.vida  -= dt * 1.8f;
        e.radio += (e.radioMax - e.radio) * dt * 4.0f;

        sf::Uint8 alpha = static_cast<sf::Uint8>(255 * e.vida);

        e.anillo.setRadius(e.radio);
        e.anillo.setOrigin(e.radio, e.radio);
        e.anillo.setOutlineColor(sf::Color(255, 150, 0, alpha));

        float rc = e.radio * 0.3f;
        e.centro.setRadius(rc);
        e.centro.setOrigin(rc, rc);
        e.centro.setFillColor(sf::Color(255, 220, 100, alpha));
    }

    for (int i = explosiones.size() - 1; i >= 0; i--)
        if (explosiones[i].vida <= 0)
            explosiones.erase(explosiones.begin() + i);

    // Nueva explosión aleatoria de fondo cada 2-3 segundos
    if (relojExplos.getElapsedTime().asSeconds() > 1.5f + (rand()%20)*0.1f) {
        generarExplosion(
            100 + rand() % (ancho - 200),
            100 + rand() % (alto  - 200)
        );
        relojExplos.restart();
    }
}

void PantallaInicio::actualizarChispas(float dt) {
    for (auto& c : chispas) {
        c.vida -= dt / c.vidaMax;

        c.forma.move(c.velocidad * dt);
        c.velocidad *= 0.92f;  // rozamiento

        sf::Uint8 alpha = static_cast<sf::Uint8>(255 * max(0.0f, c.vida));
        sf::Color col   = c.forma.getFillColor();
        col.a           = alpha;
        c.forma.setFillColor(col);
    }

    for (int i = chispas.size() - 1; i >= 0; i--)
        if (chispas[i].vida <= 0)
            chispas.erase(chispas.begin() + i);
}

// ─────────────────────────────────────────────────────────────────────────────
// Eventos y loop público
// ─────────────────────────────────────────────────────────────────────────────

void PantallaInicio::manejarEvento(sf::Event& evento,
                                    sf::Vector2i mousePos) {
    botonJugar->manejarEvento(evento, mousePos);
    botonSalir->manejarEvento(evento, mousePos);
}

void PantallaInicio::actualizar(float dt, sf::Vector2i mousePos) {
    botonJugar->actualizar(mousePos);
    botonSalir->actualizar(mousePos);

    actualizarTitulo(dt);
    actualizarBalasFondo(dt);
    actualizarExplosiones(dt);
    actualizarChispas(dt);
}

// ─────────────────────────────────────────────────────────────────────────────
// Dibujo
// ─────────────────────────────────────────────────────────────────────────────

void PantallaInicio::dibujarFondo(sf::RenderWindow& v) {
    if (fondoCargado)
        v.draw(spriteFondo);

    // Las grietas decorativas siguen encima de la imagen
    for (auto& g : grietas) v.draw(g);
}


void PantallaInicio::dibujarBalasFondo(sf::RenderWindow& v) {
    for (auto& b : balasFondo) {
        if (!b.activa) continue;
        v.draw(b.estela);
        v.draw(b.forma);
    }
}

void PantallaInicio::dibujarExplosiones(sf::RenderWindow& v) {
    for (auto& e : explosiones) {
        v.draw(e.anillo);
        v.draw(e.centro);
    }
}

void PantallaInicio::dibujarChispas(sf::RenderWindow& v) {
    for (auto& c : chispas) v.draw(c.forma);
}

void PantallaInicio::dibujarTitulo(sf::RenderWindow& v) {
    v.draw(lineaArriba);
    v.draw(lineaAbajo);

    for (auto& l : letras) {
        // Sombra de la letra
        sf::Text sombra = l.texto;
        sombra.setFillColor(sf::Color(0, 0, 0, 120));
        sombra.setOutlineThickness(0);
        sombra.move(3, 3);
        v.draw(sombra);

        // Letra con offset de vibración
        sf::Vector2f pos = l.texto.getPosition();
        l.texto.setPosition(pos.x + l.offsetX, pos.y + l.offsetY);
        v.draw(l.texto);
        l.texto.setPosition(pos);  // restaurar
    }
}

void PantallaInicio::dibujar(sf::RenderWindow& ventana) {
    // Orden de capas: fondo → tanques → balas → explosiones
    // → chispas → título → botones → texto
    dibujarFondo(ventana);
    dibujarBalasFondo(ventana);
    dibujarExplosiones(ventana);
    dibujarChispas(ventana);
    dibujarTitulo(ventana);

    botonJugar->dibujar(ventana);
    botonSalir->dibujar(ventana);

    ventana.draw(textoInstrucciones);
    ventana.draw(textoITCR);
}