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
#include <sstream>

class Objeto{
public:

    sf::Vector2f ObjPOS;
    std::string tipo;
    int bodyIndex;
    int ID;
    sf::Texture objTexture;
    std::shared_ptr<sf::Sprite> objSprite;
    b2Body* body;
    /**/
    Objeto(sf::Vector2f p): ObjPOS(p){
        objSprite = std::make_shared<sf::Sprite>();
        objSprite->setPosition(ObjPOS);
        
    }

};

//engine
class Engine{
public:

    /* #region variaveis*/

    //variaveis que guardam objetos
    //std::vector<std::shared_ptr<minerador>> mineradores; 
    //std::vector<std::shared_ptr<Bloco>> blocos;
    std::vector<std::shared_ptr<Objeto>> objetos;
    float accumulator = 0.0f;

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
    float LastTime = 0;
    double fps;
    float PPM = 0.1;
    float MPP = 1/PPM;
    float pi = 3.14159265;

    int index = 0;
    /* #endregion */

    /* #region funcoes da engine*/
    Engine();
    void draw();
    void events();
    void run();
    void EngineRUN();
    void criaRetangulo(float x, float y, float w, float h, bool dinamico);
    b2Body* criarPoligono(std::string tipo, bool dinamico,int x, int y);
    void criarOjeto(std::string tipo,float x, float y);
    //b2BodyDef createModel(std::string tipo,bool estaticoDinamico = false,float x , float y);
    /* #endregion*/

};

#endif
