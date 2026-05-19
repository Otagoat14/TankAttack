//
// Created by nacho on 5/12/2026.
//

#include "TankRenderer.h"

TankRenderer::TankRenderer(MapRender& mapaRender) : mapaRender(mapaRender)
{
    texturas[0].loadFromFile("Sprites/tanqueAz.png");   // AZUL
    texturas[1].loadFromFile("Sprites/tanqueCe.png");   // CELESTE
    texturas[2].loadFromFile("Sprites/tanqueRj.png");   // ROJO
    texturas[3].loadFromFile("Sprites/tanqueAm.png");   // AMARILLO

    for (int i = 0; i < 4; i++)
    {
        sprites[i].setTexture(texturas[i]);
    }
};

int TankRenderer::giveColor(Color color)
{
    switch (color)
    {
        case Color::AZUL : return 0;
        case Color::CELESTE : return 1;
        case Color::ROJO : return 2;
        case Color::AMARILLO : return 3;
        default: return 0;
    }
};

int TankRenderer::getColumna(Direccion dir)
{
    switch (dir)
    {
        case Direccion::SUR : return 0;
        case Direccion::NORTE : return 1;
        case Direccion::ESTE : return 2;
        case Direccion::OESTE: return 3;
        default: return 0;
    }
};


sf::IntRect TankRenderer::calcPosicion(Direccion dir)
{
    int x = getColumna(dir) * (spriteAncho + 5) + 5;
    int y = 0;
    return sf::IntRect(x, y, spriteAncho, spriteAlto);
};

void TankRenderer::dibujarTanques(sf::RenderWindow& ventana, Tank** tanques, int numTanques)
{
    for (int i = 0; i < numTanques; i++)
    {
        if (!tanques[i]->estaVivo()) continue;

        int idx = giveColor(tanques[i]->getColor());
        sprites[idx].setTextureRect(calcPosicion(tanques[i]->getDireccion()));

        sf::Vector2f pos = mapaRender.obtenerCentro(tanques[i]->getY(), tanques[i]->getX());

        float escala = (mapaRender.getCellSize() * 2) / (float)spriteAncho;
        sprites[idx].setScale(escala, escala);
        sprites[idx].setPosition(
            pos.x - (spriteAncho * escala) / 2.f,
            pos.y - (spriteAlto  * escala) / 2.f
        );
        ventana.draw(sprites[idx]);
    }
}

TankRenderer::~TankRenderer()
{

}
