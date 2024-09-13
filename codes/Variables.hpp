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
#include "lib/noise/FastNoiseLite.h"
#include <utility>
#include <set>



class MeuContactListener : public b2ContactListener {
public:
    std::set<std::pair<b2Body*, b2Body*>> contatosAtuais;

    void BeginContact(b2Contact* contact) override {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        contatosAtuais.insert({bodyA, bodyB});
        
        
    }   

    void EndContact(b2Contact* contact) override {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        contatosAtuais.erase({bodyA, bodyB});
    }

    bool ChecarColisao(b2Body* bodyA, b2Body* bodyB) {
        return contatosAtuais.find({bodyA, bodyB}) != contatosAtuais.end();
    }

};


class Objeto{
public:

    sf::Vector2f ObjPOS;
    std::string tipo;
    float Vida;
    float Dano;
    int bodyIndex;
    int ID;
    sf::Texture objTexture;
    std::shared_ptr<sf::Sprite> objSprite;
    b2Body* body;
    bool destruir = false;
    std::string grupo;
    bool arrastandoMinerador = false;
    sf::Vector2f posicaoInicialArraste;
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
    float maxDown = 46000;
    float cameraDesc = 0;
    int altura = 1000;
    int larg = 15;
    int seed = 2222222;
    bool travarCamera = false;

    std::vector<std::vector<int>> MatrizTerreno;

    //fisica BOX2D
    
    std::shared_ptr<b2World> mundo;
    sf::Clock clock;
    float deltaTime;
    float LastTime = 0;
    double fps;
    float PPM = 0.05;
    float MPP = 1/PPM;
    float pi = 3.14159265;
    bool desenharFisica = false;
    bool iniciarFisica = false;
    MeuContactListener meuContactListener;

    int index = 0;
    /* #endregion */

    /* #region funcoes da engine*/
    Engine();
    void draw();
    void events();
    void run();
    void EngineRUN();
    b2Body* criaRetangulo(float x, float y, float w, float h, bool dinamico);
    b2Body* criarPoligono(std::string tipo, bool dinamico,int x, int y);
    void criarOjeto(std::string tipo,float x, float y);
    void gerarTerreno();
    void drawPolygon(const b2PolygonShape& shape, const b2Body* body);
    void aumentarMapa(int tamanho);
    void destruirObjetos();

    //b2BodyDef createModel(std::string tipo,bool estaticoDinamico = false,float x , float y);
    /* #endregion*/







};

#endif
