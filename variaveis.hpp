#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>

class minerador{
public:
    bool pegando = false; 
    sf::Vector2i posMouseQuandoPegado;
    int ID = 0;
    float vida = 10;
    float dano = 1;
    float elasticidade = 1;
    bool criado = false;
    bool gravidade = false;
    bool animado = false;
    int frame;

    std::string tipo;
    sf::Vector2f Mineradorcords;
    sf::Vector2f Mineradorveloc;

    std::shared_ptr<sf::Sprite> mineradorSprt;
    sf::Texture mineradorTx;

    bool checarMouse(const sf::Vector2i& mousepos){
        return mineradorSprt->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousepos));
    }
    //na hora de declarar o minerador
    minerador(
        int id = 0,
        sf::Vector2f p = {0.0f, 0.0f},
        sf::Vector2f v = {0.0f, 0.0f},
        std::string t = "pa1"
    ) : ID(id), Mineradorcords(p), Mineradorveloc(v), tipo(t) {
        mineradorSprt = std::make_shared<sf::Sprite>();

        if(tipo == "pa1") {
            mineradorTx.loadFromFile("./recursos/pa_1.png");
            mineradorSprt->setTexture(mineradorTx);

            sf::FloatRect bounds = mineradorSprt->getLocalBounds();
            mineradorSprt->setOrigin(4, 15);

        }
        mineradorSprt->setPosition(Mineradorcords);
    }

   
    //para ficar facil na hora de fazer - minerador({0,0},{0,0}) posição, velocidade



};
class jogo{
public:
    float scrollSpeed = 20.0f;
    bool teclaPPressionada = false;
    std::vector<std::shared_ptr<minerador>> mineradores; 
    std::shared_ptr<sf::Sprite> bgSprite;
    std::shared_ptr<sf::RenderWindow> janela;
    sf::Texture FundoTxt;
    bool tecla = false;
    //std::unordered_map<int , std::shared_ptr<minerador>> mineradores;
    int npa = 0;
    int proximoId = 1;
    sf::View camera;




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
    void scrolle(const sf::Event::MouseWheelScrollEvent& event);
};

