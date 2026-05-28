#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;

//Colores de todas las pantallas
namespace UIColores {
    // Fondos y bases
    const sf::Color FONDO_BASE      = sf::Color(20, 14, 8);        // café muy oscuro
    const sf::Color PANEL_OSCURO    = sf::Color(30, 22, 12, 210);  // café oscuro semitransparente

    // Colores principales
    const sf::Color NARANJA_FUEGO   = sf::Color(255, 120, 30);     // naranja cálido
    const sf::Color AMARILLO_ARENA  = sf::Color(230, 185, 80);     // amarillo arena
    const sf::Color VERDE_MILITAR   = sf::Color(85, 110, 45);      // verde olivo
    const sf::Color VERDE_CLARO     = sf::Color(140, 175, 60);     // verde más brillante

    // Bordes y metales
    const sf::Color METAL_OSCURO    = sf::Color(45, 38, 25);       // café metálico oscuro
    const sf::Color METAL_CLARO     = sf::Color(140, 118, 72);     // dorado opaco
    const sf::Color METAL_BRILLO    = sf::Color(190, 160, 90);     // dorado brillante

    // Textos
    const sf::Color BLANCO_DESG     = sf::Color(230, 215, 180);    // blanco cálido desgastado
    const sf::Color GRIS_ARENA      = sf::Color(160, 145, 110);    // gris arena

    // Hover y acentos
    const sf::Color HOVER_JUGAR     = sf::Color(60, 85, 25);       // verde oscuro hover
    const sf::Color HOVER_SALIR     = sf::Color(100, 30, 15);      // rojo tierra hover
    const sf::Color ACENTO_ROJO     = sf::Color(200, 65, 30);      // rojo ladrillo
    const sf::Color ACENTO_AZUL     = sf::Color(50, 90, 160);      // azul acero

    // Compatibilidad con código existente
    const sf::Color AMARILLO_NEON   = sf::Color(230, 185, 80);     // reemplazado por arena
    const sf::Color VERDE_RADIO     = sf::Color(140, 175, 60);     // reemplazado por militar
    const sf::Color METAL_CLARO_OLD = sf::Color(140, 118, 72);
    const sf::Color ROJO            = sf::Color(200, 65, 30);
    const sf::Color AZUL            = sf::Color(50, 90, 160);
}
class UIManager {
    sf::Font fuente;

    bool fuenteCargada;

    UIManager() {
        fuenteCargada = fuente.loadFromFile(
            "../Recursos/Fuentes/PressStart2P-Regular.ttf"
        );
        if (!fuenteCargada)
            cout << "Error: no se pudo cargar la fuente" << endl;

    }

public:
    static UIManager& instancia() {
        static UIManager inst;
        return inst;
    }

    const sf::Font& getFuente() const { return fuente; }


    // Eliminar copias
    UIManager(const UIManager&)            = delete;
    UIManager& operator=(const UIManager&) = delete;
};

#endif