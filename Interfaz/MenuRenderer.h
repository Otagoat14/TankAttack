#ifndef MENURENDERER_H
#define MENURENDERER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

enum class ResultadoMenu {
    JUGAR,
    SALIR,
    NINGUNO
};

// Estructura para los puntos de impacto decorativos
struct PuntoImpacto {
    sf::CircleShape estrella;
    float opacidad;
};

// Estructura para los tanques decorativos
struct TanqueDecorativo {
    sf::RectangleShape cuerpo;
    sf::RectangleShape canon;
    sf::CircleShape rueda1;
    sf::CircleShape rueda2;
    sf::CircleShape rueda3;
    sf::Color color;
};

class MenuRenderer {
    // Fuente
    sf::Font fuente;

    // Fondo camuflaje
    std::vector<sf::RectangleShape> patronCamuflaje;

    // Título con efecto glow/sombra
    sf::Text tituloSombra1;
    sf::Text tituloSombra2;
    sf::Text titulo;
    sf::RectangleShape bordesTitulo[4];  // bordes metálicos

    // Botones
    sf::RectangleShape fondoJugar;
    sf::RectangleShape brilloJugar;
    sf::Text textoJugar;

    sf::RectangleShape fondoSalir;
    sf::RectangleShape brilloSalir;
    sf::Text textoSalir;

    // Instrucciones
    sf::Text instrucciones;

    // Info institucional
    sf::Text infoITCR;

    // Tanques decorativos
    TanqueDecorativo tanqueRojo;
    TanqueDecorativo tanqueAzul;

    // Puntos de impacto
    std::vector<PuntoImpacto> puntosImpacto;

    // Animación parpadeo título
    sf::Clock relojParpadeo;
    sf::Clock relojGlow;
    bool tituloVisible;
    float glowIntensidad;

    // Dimensiones ventana
    int ancho;
    int alto;

    // Colores
    sf::Color COLOR_FONDO_BASE    = sf::Color(27, 30, 15);
    sf::Color COLOR_CAMU_1        = sf::Color(75, 83, 32);
    sf::Color COLOR_CAMU_2        = sf::Color(40, 44, 20);
    sf::Color COLOR_CAMU_3        = sf::Color(55, 60, 25);
    sf::Color COLOR_AMARILLO_NEON = sf::Color(255, 220, 0);
    sf::Color COLOR_VERDE_RADIO   = sf::Color(0, 255, 80);
    sf::Color COLOR_METAL_OSCURO  = sf::Color(50, 55, 50);
    sf::Color COLOR_METAL_CLARO   = sf::Color(120, 130, 110);
    sf::Color COLOR_ROJO_TANQUE   = sf::Color(200, 40, 40);
    sf::Color COLOR_AZUL_TANQUE   = sf::Color(40, 80, 200);
    sf::Color COLOR_BLANCO_DESG   = sf::Color(220, 215, 195);

    // Métodos privados de construcción
    void construirCamuflaje();
    void construirTitulo();
    void construirBotones();
    void construirTanqueDecorativo(TanqueDecorativo& t,
                                   float x, float y,
                                   sf::Color color,
                                   bool mirandoDerecha);
    void construirPuntosImpacto();
    void construirInstrucciones();
    void construirInfoITCR();

    // Métodos de actualización
    void actualizarParpadeo();
    void actualizarHover(sf::Vector2i mousePos);

    // Helpers
    bool estaEncima(sf::RectangleShape& forma, sf::Vector2i mousePos);
    void centrarTexto(sf::Text& texto, float y);
    void dibujarTanque(sf::RenderWindow& ventana, TanqueDecorativo& t);

public:
    MenuRenderer(int anchoVentana, int altoVentana);

    ResultadoMenu manejarEvento(sf::Event& evento, sf::Vector2i mousePos);
    void actualizar();
    void dibujar(sf::RenderWindow& ventana);
};

#endif