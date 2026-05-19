#include "PantallaJuego.h"

using namespace std;

static const int ALTO_PANEL_SUP = 64;
static const int ALTO_PANEL_INF = 110;
static const int MARGEN         = 12;

// ─────────────────────────────────────────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────

PantallaJuego::PantallaJuego(int ancho, int alto,
                               function<void()> onMenuPrincipal)
    : ancho(ancho), alto(alto)
{
    tanqueSeleccionado = -1;
    construirFondo();
    construirPanelSuperior();
    construirAreaMapa();
    construirPanelInferior();
    construirMapa();
    construirTanques();
    //construirPanelPowerUps();

    botonMenu = new Boton(
        ancho - 70.0f, 32.0f,   // posición (centro del botón)
        110, 38,
        "MENU",
        onMenuPrincipal,
        UIColores::PANEL_OSCURO,
        UIColores::HOVER_SALIR,
        UIColores::METAL_CLARO,
        UIColores::ACENTO_ROJO,
        UIColores::BLANCO_DESG,
        UIColores::ACENTO_ROJO
    );
}

PantallaJuego::~PantallaJuego() {
    for (int i = 0; i < 2; i++) {
        delete barraVidaJ1[i];
        delete barraVidaJ2[i];
    }
    delete botonMenu;
    delete mapRender;
    delete mapa;
    delete grafo;
    for (int i = 0; i < 4; i++) delete tanques[i];
    delete tankRenderer;
}

// ─────────────────────────────────────────────────────────────────────────────
// Construcción del fondo
// ─────────────────────────────────────────────────────────────────────────────

void PantallaJuego::construirFondo() {
    fondoGeneral.setSize(sf::Vector2f(ancho, alto));
    fondoGeneral.setPosition(0, 0);
    fondoGeneral.setFillColor(UIColores::FONDO_BASE);
}

// ─────────────────────────────────────────────────────────────────────────────
// Panel superior
// ─────────────────────────────────────────────────────────────────────────────

void PantallaJuego::construirPanelSuperior() {
    // Fondo del panel
    panelSuperior.setSize(sf::Vector2f(ancho, ALTO_PANEL_SUP));
    panelSuperior.setPosition(0, 0);
    panelSuperior.setFillColor(UIColores::PANEL_OSCURO);
    panelSuperior.setOutlineColor(UIColores::METAL_CLARO);
    panelSuperior.setOutlineThickness(1);

    // ── Jugador 1 (lado izquierdo) ──
    textoLabelJ1.setFont(UIManager::instancia().getFuente());
    textoLabelJ1.setString("JUGADOR 1");
    textoLabelJ1.setCharacterSize(13);
    textoLabelJ1.setFillColor(sf::Color(200, 80, 60));
    textoLabelJ1.setPosition((ancho/2.0f) - 220, 10);

    // ── Jugador 2 (lado derecho) ──
    textoLabelJ2.setFont(UIManager::instancia().getFuente());
    textoLabelJ2.setString("JUGADOR 2");
    textoLabelJ2.setCharacterSize(13);
    textoLabelJ2.setFillColor(sf::Color(60, 160, 200));
    // Alinear a la derecha (dejando espacio para el botón MENU)
    sf::FloatRect bJ2 = textoLabelJ2.getLocalBounds();
    textoLabelJ2.setPosition((ancho/2.0f) + 100, 10);

    // ── Tiempo (centro) ──
    textoLabelTiempo.setFont(UIManager::instancia().getFuente());
    textoLabelTiempo.setString("TIEMPO");
    textoLabelTiempo.setCharacterSize(7);
    textoLabelTiempo.setFillColor(UIColores::GRIS_ARENA);
    sf::FloatRect bLT = textoLabelTiempo.getLocalBounds();
    textoLabelTiempo.setOrigin(bLT.width / 2.0f, 0);
    textoLabelTiempo.setPosition(ancho / 2.0f, 8);

    textoTiempo.setFont(UIManager::instancia().getFuente());
    textoTiempo.setString("05:00");
    textoTiempo.setCharacterSize(20);
    textoTiempo.setFillColor(UIColores::BLANCO_DESG);
    sf::FloatRect bT = textoTiempo.getLocalBounds();
    textoTiempo.setOrigin(bT.width / 2.0f, 0);
    textoTiempo.setPosition(ancho / 2.0f, 22);
}

// ─────────────────────────────────────────────────────────────────────────────
// Área del mapa (placeholder)
// ─────────────────────────────────────────────────────────────────────────────

void PantallaJuego::construirAreaMapa() {
    float yInicio  = ALTO_PANEL_SUP + MARGEN;
    float altoArea = alto - ALTO_PANEL_SUP - ALTO_PANEL_INF - MARGEN * 2;
    float anchoArea = ancho - MARGEN * 2;

    // Borde exterior decorativo
    bordeMapa.setSize(sf::Vector2f(anchoArea + 6, altoArea + 6));
    bordeMapa.setPosition(MARGEN - 3, yInicio - 3);
    bordeMapa.setFillColor(sf::Color::Transparent);
    bordeMapa.setOutlineColor(UIColores::METAL_CLARO);
    bordeMapa.setOutlineThickness(3);

    // Área interior (aquí se dibujará el mapa)
    areaMapaFondo.setSize(sf::Vector2f(anchoArea, altoArea));
    areaMapaFondo.setPosition(MARGEN, yInicio);
    areaMapaFondo.setFillColor(sf::Color(45, 38, 25));   // café oscuro tierra
    areaMapaFondo.setOutlineColor(sf::Color(30, 25, 15));
    areaMapaFondo.setOutlineThickness(1);

    // Texto placeholder centrado
    textoPlaceholderMapa.setFont(UIManager::instancia().getFuente());
    textoPlaceholderMapa.setString("[ MAPA ]");
    textoPlaceholderMapa.setCharacterSize(10);
    textoPlaceholderMapa.setFillColor(sf::Color(80, 70, 50, 160));
    sf::FloatRect bP = textoPlaceholderMapa.getLocalBounds();
    textoPlaceholderMapa.setOrigin(bP.width / 2.0f, bP.height / 2.0f);
    textoPlaceholderMapa.setPosition(
        MARGEN + anchoArea / 2.0f,
        yInicio + altoArea / 2.0f
    );
}

void PantallaJuego::construirMapa()
{
    grafo = new Graph(18, 43);
    mapa  = new Map(*grafo, 18, 43, 20);
    mapa->doMapa();
    mapRender = new MapRender(*mapa, *grafo,
        MARGEN, ALTO_PANEL_SUP + MARGEN,
        ancho - MARGEN * 2,
        alto - ALTO_PANEL_SUP - ALTO_PANEL_INF - MARGEN * 2
    );
};

void PantallaJuego::construirTanques() {
    int cols = grafo->getCols();
    int rows = grafo->getRows();

    int c0, f0, c1, f1, c2, f2, c3, f3;

    mapRender->encontrarPosLibre(1,        1,            true,  c0, f0);
    mapRender->encontrarPosLibre(1,        rows / 2,     true,  c1, f1);
    mapRender->encontrarPosLibre(cols - 2, 1,            false, c2, f2);
    mapRender->encontrarPosLibre(cols - 2, rows / 2,     false, c3, f3);

    tanques[0] = new tankBFS     (c0, f0, 100, Equipo::JUGADOR1, Color::ROJO);
    tanques[1] = new tankBFS     (c1, f1, 100, Equipo::JUGADOR1, Color::AZUL);
    tanques[2] = new tankDijkstra(c2, f2, 100, Equipo::JUGADOR2, Color::AMARILLO);
    tanques[3] = new tankDijkstra(c3, f3, 100, Equipo::JUGADOR2, Color::CELESTE);

    tanques[0]->setDireccion(Direccion::OESTE);
    tanques[1]->setDireccion(Direccion::OESTE);
    tanques[2]->setDireccion(Direccion::ESTE);
    tanques[3]->setDireccion(Direccion::ESTE);

    tankRenderer = new TankRenderer(*mapRender);
}

// ─────────────────────────────────────────────────────────────────────────────
// Panel inferior
// ─────────────────────────────────────────────────────────────────────────────

void PantallaJuego::construirPanelInferior() {
    float yInf = alto - ALTO_PANEL_INF;

    panelInferior.setSize(sf::Vector2f(ancho, ALTO_PANEL_INF));
    panelInferior.setPosition(0, yInf);
    panelInferior.setFillColor(UIColores::PANEL_OSCURO);
    panelInferior.setOutlineColor(UIColores::METAL_CLARO);
    panelInferior.setOutlineThickness(1);

    // Línea divisora central entre J1 y J2
    separadorInferior.setSize(sf::Vector2f(2, ALTO_PANEL_INF - 20));
    separadorInferior.setPosition(ancho / 2.0f - 1, yInf + 10);
    separadorInferior.setFillColor(UIColores::METAL_CLARO);

    // ── Configuración de tanques ──
    // Cada mitad del panel tiene 2 slots de tanque.
    // Layout por slot:  [icono 36x36] + [etiqueta + barra de vida]
    // Los 2 slots de J1 van en la mitad izquierda,
    // los 2 slots de J2 en la mitad derecha.

    float mitad     = ancho / 2.0f;
    float slotAncho = (mitad - MARGEN * 3) / 2.0f;  // ancho de cada slot
    float iconoSide = 36;
    float barraAncho = slotAncho - iconoSide - 12;
    float yBase      = yInf + 16;

    // Colores de los tanques por jugador
    sf::Color coloresJ1[2] = { sf::Color(200, 50, 30),  sf::Color(40, 80, 200)  };
    sf::Color coloresJ2[2] = { sf::Color(220, 185, 30), sf::Color(50, 185, 200) };
    string nombresJ1[2] = { "ROJO", "AZUL" };
    string nombresJ2[2] = { "AMAR", "CELES" };

    for (int i = 0; i < 2; i++) {
        // ── Jugador 1 ──
        float xSlotJ1 = MARGEN + i * (slotAncho + MARGEN);

        iconoTanqueJ1[i].setSize(sf::Vector2f(iconoSide, iconoSide));
        iconoTanqueJ1[i].setPosition(xSlotJ1, yBase);
        iconoTanqueJ1[i].setFillColor(
            sf::Color(coloresJ1[i].r, coloresJ1[i].g, coloresJ1[i].b, 180));
        iconoTanqueJ1[i].setOutlineColor(coloresJ1[i]);
        iconoTanqueJ1[i].setOutlineThickness(2);

        labelTanqueJ1[i].setFont(UIManager::instancia().getFuente());
        labelTanqueJ1[i].setString(nombresJ1[i]);
        labelTanqueJ1[i].setCharacterSize(7);
        labelTanqueJ1[i].setFillColor(UIColores::BLANCO_DESG);
        labelTanqueJ1[i].setPosition(xSlotJ1 + iconoSide + 6, yBase);

        barraVidaJ1[i] = new BarraVida(
            xSlotJ1 + iconoSide + 6,
            yBase + 16,
            barraAncho, 14,
            100, ""
        );

        // ── Jugador 2 ──
        float xSlotJ2 = mitad + MARGEN + i * (slotAncho + MARGEN);

        iconoTanqueJ2[i].setSize(sf::Vector2f(iconoSide, iconoSide));
        iconoTanqueJ2[i].setPosition(xSlotJ2, yBase);
        iconoTanqueJ2[i].setFillColor(
            sf::Color(coloresJ2[i].r, coloresJ2[i].g, coloresJ2[i].b, 180));
        iconoTanqueJ2[i].setOutlineColor(coloresJ2[i]);
        iconoTanqueJ2[i].setOutlineThickness(2);

        labelTanqueJ2[i].setFont(UIManager::instancia().getFuente());
        labelTanqueJ2[i].setString(nombresJ2[i]);
        labelTanqueJ2[i].setCharacterSize(7);
        labelTanqueJ2[i].setFillColor(UIColores::BLANCO_DESG);
        labelTanqueJ2[i].setPosition(xSlotJ2 + iconoSide + 6, yBase);

        barraVidaJ2[i] = new BarraVida(
            xSlotJ2 + iconoSide + 6,
            yBase + 16,
            barraAncho, 14,
            100, ""
        );
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Panel de power-ups
// ─────────────────────────────────────────────────────────────────────────────

void PantallaJuego::construirPanelPowerUps() {
    float yInf      = alto - ALTO_PANEL_INF;
    float anchoPow  = 180;
    float altoPow   = ALTO_PANEL_INF - 20;
    float yPow      = yInf + 10;

    // Power-ups J1: pegado a la derecha de los slots de J1
    float xPowJ1 = ancho / 2.0f - anchoPow - MARGEN - 2;
    panelPowerUpJ1.setSize(sf::Vector2f(anchoPow, altoPow));
    panelPowerUpJ1.setPosition(xPowJ1, yPow);
    panelPowerUpJ1.setFillColor(sf::Color(30, 25, 15, 180));
    panelPowerUpJ1.setOutlineColor(UIColores::METAL_OSCURO);
    panelPowerUpJ1.setOutlineThickness(1);

    labelPowerUpJ1.setFont(UIManager::instancia().getFuente());
    labelPowerUpJ1.setString("POWER-UPS: -");
    labelPowerUpJ1.setCharacterSize(7);
    labelPowerUpJ1.setFillColor(UIColores::GRIS_ARENA);
    labelPowerUpJ1.setPosition(xPowJ1 + 6, yPow + 8);

    // Power-ups J2: pegado a la izquierda de los slots de J2
    float xPowJ2 = ancho / 2.0f + MARGEN + 2;
    panelPowerUpJ2.setSize(sf::Vector2f(anchoPow, altoPow));
    panelPowerUpJ2.setPosition(xPowJ2, yPow);
    panelPowerUpJ2.setFillColor(sf::Color(30, 25, 15, 180));
    panelPowerUpJ2.setOutlineColor(UIColores::METAL_OSCURO);
    panelPowerUpJ2.setOutlineThickness(1);

    labelPowerUpJ2.setFont(UIManager::instancia().getFuente());
    labelPowerUpJ2.setString("POWER-UPS: -");
    labelPowerUpJ2.setCharacterSize(7);
    labelPowerUpJ2.setFillColor(UIColores::GRIS_ARENA);
    labelPowerUpJ2.setPosition(xPowJ2 + 6, yPow + 8);
}

// ─────────────────────────────────────────────────────────────────────────────
// Eventos / Actualización / Dibujo
// ─────────────────────────────────────────────────────────────────────────────

void PantallaJuego::manejarEvento(sf::Event& evento,
                                   sf::Vector2i mousePos) {
    botonMenu->manejarEvento(evento, mousePos);
    if (evento.type == sf::Event::MouseButtonPressed) {
        if (evento.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i celda = mapRender->obtenerCelda(mousePos.x, mousePos.y);
            manejarClick(celda.x, celda.y);
        }
    }
}

void PantallaJuego::manejarClick(int col, int row)
{
    if (tanqueSeleccionado == -1)
    {
        for (int i = 0; i < 4; i++)
        {
            if(tanques[i]->getX() == col  &&  tanques[i]->getY() == row)
            {
                tanqueSeleccionado = i;
            }
        }
    } else {
        tanques[tanqueSeleccionado]->moverse(col, row, *grafo, *mapa);
        tanqueSeleccionado = -1;
    }
}


void PantallaJuego::actualizar(float dt, sf::Vector2i mousePos) {
    botonMenu->actualizar(mousePos);

    //Manntiene constantemente los tanques siendo actualizados
    for (int i = 0; i < 4; i++) {
        if (tanques[i] != nullptr)
            tanques[i]->actualizar(dt);
    }
    // La lógica del juego (tiempo, turno, movimiento, etc.) se añade aquí después
}

// ── Dibujo por secciones ────────────────────────────────────────────────────

void PantallaJuego::dibujarPanelSuperior(sf::RenderWindow& v) {
    v.draw(panelSuperior);
    v.draw(separadorCentro);
    v.draw(textoLabelJ1);
    v.draw(textoLabelJ2);
    v.draw(textoLabelTiempo);
    v.draw(textoTiempo);
    botonMenu->dibujar(v);
}

void PantallaJuego::dibujarAreaMapa(sf::RenderWindow& v) {
    v.draw(bordeMapa);
    v.draw(areaMapaFondo);
    mapRender->dibujar(v);
    tankRenderer->dibujarTanques(v, tanques, 4);
}

void PantallaJuego::dibujarPanelInferior(sf::RenderWindow& v) {
    v.draw(panelInferior);
    v.draw(separadorInferior);

    for (int i = 0; i < 2; i++) {
        v.draw(iconoTanqueJ1[i]);
        v.draw(labelTanqueJ1[i]);
        barraVidaJ1[i]->dibujar(v);

        v.draw(iconoTanqueJ2[i]);
        v.draw(labelTanqueJ2[i]);
        barraVidaJ2[i]->dibujar(v);
    }
}

void PantallaJuego::dibujarPanelPowerUps(sf::RenderWindow& v) {
    v.draw(panelPowerUpJ1);
    v.draw(labelPowerUpJ1);
    v.draw(panelPowerUpJ2);
    v.draw(labelPowerUpJ2);
}

void PantallaJuego::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(fondoGeneral);
    dibujarAreaMapa(ventana);
    dibujarPanelInferior(ventana);
    dibujarPanelPowerUps(ventana);
    dibujarPanelSuperior(ventana);
}