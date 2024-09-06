/*

 _____                 _ _   _               _ _             
|  __ \               (_) | | |             | (_)            
| |  \/_ __ __ ___   ___| |_| |_ _   _    __| |_  __ _  __ _ 
| | __| '__/ _` \ \ / / | __| __| | | |  / _` | |/ _` |/ _` |
| |_\ \ | | (_| |\ V /| | |_| |_| |_| | | (_| | | (_| | (_| |
 \____/_|  \__,_| \_/ |_|\__|\__|\__, |  \__,_|_|\__, |\__, |
                                  __/ |           __/ | __/ |
                                 |___/           |___/ |___/ 

    um jogo desesnvolvido do 0 por um programador inexperiente

*/

#include "Variables.hpp"


//construtor
Engine::Engine()
{
    
    /*#region definir janela*/
    janela = std::make_shared<sf::RenderWindow>(sf::VideoMode(800,600),"gravity digg cpp");
    janela->setFramerateLimit(60);
    camera.setSize(sf::Vector2f(janela->getSize()));
    camera.setCenter(400,300);
    janela->setView(camera);
    /*#endregion*/

    /*#region definir fundo*/
    bgSprite = std::make_shared<sf::Sprite>();
    FundoTxt.loadFromFile("./recursos/textures/Background/BG1.png");
    bgSprite->setTexture(FundoTxt);
    /*#endregion*/

    /*#region definir mundo da fisica*/
    b2Vec2 grav(0,-10);
    mundo = std::make_shared<b2World>(grav);
    /*#endregion*/

}

//desenhar elementos na tela
void Engine::draw()
{

    janela->clear(sf::Color::Black);

    janela->draw(*bgSprite);

    janela->display();

    
}

//eventos(inputs do usuario)
void Engine::events()
{
    
    sf::Event evento;
    
    //arvore de eventos
    while (janela->pollEvent(evento))
    {

        //fechar janela
        if(evento.type == sf::Event::Closed){
            janela->close();
        }
        
        //scroll camera
        if (evento.type == sf::Event::MouseWheelScrolled) {
                std::cout << "Scroll";
                sf::Vector2f cameraPos = camera.getCenter();
                if (evento.mouseWheelScroll.delta > 0) {
                    std::cout << "Scroll Up\n";
                    // Aumenta o tamanho da visão (zoom out)
                    if (cameraPos.y > 300){
                        camera.move(0, -scrollSpeed);  // Move a câmera para cima (scroll up)
                    }
                } else if (evento.mouseWheelScroll.delta < 0) {
                    std::cout << "Scroll Down\n";
                    // Diminui o tamanho da visão (zoom in)
                    
                    camera.move(0, scrollSpeed);  // Move a câmera para baixo (scroll down)
                    
                }
                janela->setView(camera);
        }

    }




}

//função que junta tudo para rodar
void Engine::run()
{

    clock.restart();  

    while(janela->isOpen()){
        



        events();
        draw();
    }
}














/*
b2BodyDef Engine::createModel(std::string tipo,bool estaticoDinamico = false,float x , float y){
    std::fstream arquivo("./recursos/ObjectsJSON/models.json");

    b2BodyDef Construtor;
    nlohmann::json jsondata;
    arquivo >> jsondata;

    for (const auto& body : jsondata["bodydef"]["bodies"]["body"]){
        if(body["_name"] == tipo){
            auto fixture = body["fixtures"]["fixture"];
            auto polygons = fixture["polygons"]["polygon"];


        }
        
    }
}
*/


