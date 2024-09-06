#ifndef VARIABLES_HPP
#define VARIABLES_HPP


#include <string>
#include <memory>
#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>
#include <random>
#include "lib/PixelPerfectCollision/Collision.hpp"
#include <chrono>
#include "box2d/box2d.h"
#include "nlohmann/json.hpp"
#include <fstream>

class Objetos{





};

//engine
class Engine{
public:

    /* #region variaveis*/

    //variaveis que guardam objetos
    //std::vector<std::shared_ptr<minerador>> mineradores; 
    //std::vector<std::shared_ptr<Bloco>> blocos;

    //textura do fundo
    std::shared_ptr<sf::Sprite> bgSprite,ponteiroSprite;
    std::shared_ptr<sf::RenderWindow> janela;
    sf::Texture FundoTxt;

    //camera/eventos
    sf::View camera;
    float scrollSpeed = 20.0f;
    bool tecla = false;
    bool minerar = false;

    //fisica BOX2D
    std::shared_ptr<b2World> mundo;
    sf::Clock clock;
    float deltaTime;
    float tempoInicial = 0;
    
    /* #endregion */

    /* #region funcoes da engine*/
    Engine();
    void draw();
    void events();
    void run();
    //b2BodyDef createModel(std::string tipo,bool estaticoDinamico = false,float x , float y);
    /* #endregion*/

};

#endif
