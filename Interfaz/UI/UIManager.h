#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;

//Colores de todas las pantallas
namespace UIColores {
    const sf::Color FONDO_BASE      = sf::Color(27, 30, 15);
    const sf::Color CAMU_1          = sf::Color(75, 83, 32);
    const sf::Color CAMU_2          = sf::Color(40, 44, 20);
    const sf::Color AMARILLO_NEON   = sf::Color(255, 220, 0);
    const sf::Color VERDE_RADIO     = sf::Color(0, 255, 80);
    const sf::Color METAL_OSCURO    = sf::Color(50, 55, 50);
    const sf::Color METAL_CLARO     = sf::Color(120, 130, 110);
    const sf::Color BLANCO_DESG     = sf::Color(220, 215, 195);
    const sf::Color ROJO            = sf::Color(200, 40, 40);
    const sf::Color AZUL            = sf::Color(40, 80, 200);
    const sf::Color GRIS_OSCURO     = sf::Color(30, 35, 25);
}

class UIManager {
    sf::Font fuente;
    // sf::SoundBuffer bufferClick;  // descomentar cuando tengan el archivo
    // sf::Sound sonidoClick;

    bool fuenteCargada;

    UIManager() {
        fuenteCargada = fuente.loadFromFile(
            "../../Recursos/Fuentes/PressStart2P-Regular.ttf"
        );
        if (!fuenteCargada)
            cout << "Error: no se pudo cargar la fuente" << endl;

        // Cunao haya sonido
        // bufferClick.loadFromFile("Recursos/Sonidos/click.wav");
        // sonidoClick.setBuffer(bufferClick);
    }

public:
    static UIManager& instancia() {
        static UIManager inst;
        return inst;
    }

    const sf::Font& getFuente() const { return fuente; }

    void reproducirClick() {
        // sonidoClick.play();  // descomentar cuando se tenga el sonido de los botones
    }

    // Eliminar copias
    UIManager(const UIManager&)            = delete;
    UIManager& operator=(const UIManager&) = delete;
};

#endif