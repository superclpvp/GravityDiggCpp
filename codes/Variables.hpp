#ifndef VARIABLES_HPP
#define VARIABLES_HPP


#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>

class Bloco{
public:
    int ID;
    float vida;
    float nivel; 
    sf::Vector2f cord;
    std::shared_ptr<sf::Sprite> blocoSprt;
    sf::Texture blocoTx;
    std::string tipo;

    Bloco(std::string t,sf::Vector2f p) : tipo(t), cord(p){
        blocoSprt = std::make_shared<sf::Sprite>();

        if(tipo == "terra"){
            blocoTx.loadFromFile("./recursos/sprite/terra.png");
            blocoSprt->setTexture(blocoTx);
        }
        blocoSprt->setPosition(cord);
    }

};

class minerador{
public:

    sf::Vector2f rotacaoVeloc;
    bool pegando = false; 
    sf::Vector2i posMouseQuandoPegado;
    int ID = 0;
    float vida = 10;
    float dano = 1;
    float elasticidade = 1.6f;
    bool criado = false;
    bool animado = false;
    int frame;
    float perdaDeEnergia = 1.0f;

    std::string tipo;
    sf::Vector2f Mineradorcords;
    sf::Vector2f Mineradorveloc;

    std::shared_ptr<sf::Sprite> mineradorSprt;
    sf::Texture mineradorTx;

    // Adicione variáveis para o tamanho da janela
    float janelaLargura;
    float janelaAltura;

    minerador(
        int id = 0,
        sf::Vector2f p = {0.0f, 0.0f},
        sf::Vector2f v = {0.0f, 0.0f},
        std::string t = "pa1",
        float larguraJanela = 800.0f,  // Valor padrão
        float alturaJanela = 600.0f   // Valor padrão
    ) : ID(id), Mineradorcords(p), Mineradorveloc(v), tipo(t), janelaLargura(larguraJanela), janelaAltura(alturaJanela) {
        mineradorSprt = std::make_shared<sf::Sprite>();

        if(tipo == "pa1") {
            mineradorTx.loadFromFile("./recursos/sprite/pa_1.png");
            mineradorSprt->setTexture(mineradorTx);

            sf::FloatRect bounds = mineradorSprt->getLocalBounds();
            mineradorSprt->setOrigin(4, 15);
        }
        mineradorSprt->setPosition(Mineradorcords);
    }

    void fisica(std::vector<std::shared_ptr<Bloco>> blocos, float janelaLargura, float janelaAltura);
    void verificarColisaoBordas(sf::Vector2f posMinerador, float janelaLargura, float janelaAltura);
    bool verificarColisaoPrecisa(const sf::Sprite minerador, const sf::Sprite bloco);

    bool checarMouse(const sf::Vector2i& mousepos){
        return mineradorSprt->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousepos));
    }
};



class jogo{
public:
    bool minerar = false;
    float scrollSpeed = 20.0f;
    bool teclaPPressionada = false;
    std::vector<std::shared_ptr<minerador>> mineradores; 
    std::vector<std::shared_ptr<Bloco>> blocos;
    std::shared_ptr<sf::Sprite> bgSprite,ponteiroSprite;
    std::shared_ptr<sf::RenderWindow> janela;
    sf::Texture FundoTxt;
    bool tecla = false;
    //std::unordered_map<int , std::shared_ptr<minerador>> mineradores;
    int npa = 0;
    int proximoId = 1;
    sf::View camera;

    void criarBlocos(std::string tipoBloco,sf::Vector2f pos){
        blocos.push_back(std::make_shared<Bloco>(tipoBloco,pos));
    }


    void criarMineradores() {
        while (npa > mineradores.size()) {
            // Adiciona novos mineradores ao vetor
            mineradores.push_back(std::make_shared<minerador>(
                proximoId,
                sf::Vector2f(100.0f, 100.0f), // posição inicial
                sf::Vector2f(0.0f, 0.0f), // velocidade inicial
                "pa1"                      // tipo de minerador
            ));
            proximoId++;
            std::cout << "Numero de mineradores: " << mineradores.size() << "\n";
        }
    }

    void destruitMineradores(int id) {
        std::cout << "Tentando destruir minerador com ID: " << id << "\n";
        auto it = std::remove_if(mineradores.begin(), mineradores.end(),
            [id](const std::shared_ptr<minerador>& min) {
                return min->ID == id;
            });

        if (it != mineradores.end()) {
            mineradores.erase(it, mineradores.end());
            npa--;
            std::cout << "Minerador destruido\n";
        } else {
            std::cout << "Minerador não encontrado\n";
        }
    }
 
    jogo();
    void desenhe();
    void eventos();
    void run();
    void updateCamera();
    void gerar();
    void RunEngine();
};

#endif
