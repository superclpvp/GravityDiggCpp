#include "variaveis.hpp"
#include <map>

#include <memory>
#include <unordered_map>

jogo::jogo(){
    
    janela = std::make_shared<sf::RenderWindow>(sf::VideoMode(800,600),"gravity digg cpp");
    camera.setSize(sf::Vector2f(janela->getSize()));
    camera.setCenter(400,300);
    janela->setView(camera);

    bgSprite = std::make_shared<sf::Sprite>();
    FundoTxt.loadFromFile("./recursos/fundos/BG1.png");
    bgSprite->setTexture(FundoTxt);

    //mineradorTx.loadFromFile("recursos/pa_1.png");
    //mineradorSprt.setTexture(mineradorTx);

}

void jogo::desenhe(){

    janela->clear(sf::Color::Black);

    janela->draw(*bgSprite);

    for (auto& min : mineradores) {
        janela->draw(*min->mineradorSprt);  // Assumindo que mineradorSprt é o sprite a ser desenhado
    }
    
    
    janela->display();

    
}

void jogo::eventos(){
    
    sf::Event evento;
    
    while (janela->pollEvent(evento))
    {
        if(evento.type == sf::Event::Closed){
            janela->close();
        }
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
      //crirar minerador -temporario-
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
            if(!tecla){
                criarMineradores();
                tecla = true;
                npa++;
                std::cout<<"p \n";
            }
        }
        else{
            tecla = false;
        }
        
     //drag and drop minerador  
        if(evento.type == sf::Event::MouseButtonPressed){
            if(evento.mouseButton.button == sf::Mouse::Left){
                sf::Vector2i mousePos = sf::Mouse::getPosition(*janela);
                
                for (auto& min : mineradores) {
                    if (min->checarMouse(mousePos)) {
                        min->pegando = true;
                        min->posMouseQuandoPegado = mousePos;  
                        break;
                    }   
                }
            }

            else if(evento.mouseButton.button == sf::Mouse::Right){
                for (auto& min : mineradores) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*janela);
                    if(min->checarMouse(mousePos)){
                        destruitMineradores(min->ID);
                        break;
                    }

                }
            }
            
        }

        if (evento.type == sf::Event::MouseButtonReleased) {
            // Quando o botão do mouse é solto
            if (evento.mouseButton.button == sf::Mouse::Left) {
                // Solta o minerador
                for (auto& min : mineradores) {
                    if (min->pegando) {
                        min->pegando = false;
                        break;
        }}}}
        
        
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*janela);
        for (auto& min : mineradores) {
            if (min->pegando) {
                // Atualiza a posição do minerador com a posição do mouse
                min->mineradorSprt->setPosition(static_cast<sf::Vector2f>(mousePos));
            }
        }
    }


}

void jogo::run(){

    while(janela->isOpen()){
        eventos();
        desenhe();
        

    }
}


