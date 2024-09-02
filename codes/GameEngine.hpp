#include "Variables.hpp"
#include "physicsEngine.hpp"
#include <memory>


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
    
    for (auto& blo : blocos) {
        janela->draw(*blo->blocoSprt);
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
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !minerar){
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
        if(evento.type == sf::Event::MouseButtonPressed && !minerar){
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

            else if(evento.mouseButton.button == sf::Mouse::Right && !minerar){
                for (auto& min : mineradores) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*janela);
                    if(min->checarMouse(mousePos)){
                        destruitMineradores(min->ID);
                        break;
                    }

                }
            }
            
        }
        
        if (evento.type == sf::Event::MouseButtonReleased && !minerar) {
            // Quando o botão do mouse é solto
            
            //mousePos.x > 11 && mousePos.x < 780 && mousePos.y > 11 && mousePos < 190
            if (evento.mouseButton.button == sf::Mouse::Left) {
                // Solta o minerador
                for (auto& min : mineradores) {
                    if (min->pegando) {
                        min->pegando = false;
                        break;
        }}}}
        
        //iniciar fisica
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !minerar){
            minerar = true;
        }
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(*janela);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mousePos.x > 32 && mousePos.x < 568 && mousePos.y > 32 && mousePos.y < 130 && !minerar) {
        //sf::Vector2i mousePos = sf::Mouse::getPosition(*janela);
        for (auto& min : mineradores) {
            if (min->pegando) {
                // Atualiza a posição do minerador com a posição do mouse
                min->mineradorSprt->setPosition(static_cast<sf::Vector2f>(mousePos));
                min->Mineradorcords = min->mineradorSprt->getPosition();
            }
        }
    }


}

void jogo::gerar(){
    ponteiroSprite = std::make_shared<sf::Sprite>();

    ponteiroSprite->setPosition(0,214);//25

    for (int i = 0; i < 13; i++){
        for(int x = 0; x < 13; x++){
            criarBlocos("terra",ponteiroSprite->getPosition());
            ponteiroSprite->setPosition(0+(64*x),214+(64*i));
        }
    }

}

void jogo::RunEngine(){
    for (auto& min : mineradores) {
        min->fisica(blocos,800,600);  // Aplica a física para cada minerador
    }
}

void jogo::run(){
    
    gerar();

    while(janela->isOpen()){
        eventos();
        desenhe();
        
        if(minerar){
            RunEngine();
        }
    }
}
















