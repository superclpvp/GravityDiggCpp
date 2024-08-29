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
        if (evento.type == sf::Event::MouseWheelScrolled && !minerar) {
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
        min->fisica(blocos);  // Aplica a física para cada minerador
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

void minerador::fisica(std::vector<std::shared_ptr<Bloco>> blocos) {
    // Aplicar gravidade
    float gravidade = 0.01f;
    Mineradorveloc.y += gravidade;

    // Atualizar a posição do minerador
    Mineradorcords += Mineradorveloc;
    mineradorSprt->setPosition(Mineradorcords);

    // Aplicar rotação baseada no vetor de rotação
    mineradorSprt->rotate(rotacaoVeloc.x * rotacaoVeloc.y);

    // Verificar colisão com blocos
    for (auto& bloco : blocos) {
        if (mineradorSprt->getGlobalBounds().intersects(bloco->blocoSprt->getGlobalBounds())) {
            if (Mineradorveloc.y > 0) {
                Mineradorveloc.y *= -elasticidade;
                Mineradorcords.y = bloco->blocoSprt->getPosition().y - mineradorSprt->getGlobalBounds().height;  // Ajusta a posição para não entrar no bloco
            }
            if (Mineradorveloc.x != 0) {
                Mineradorveloc.x *= -elasticidade;
            }
        }
    }

    // Verificar colisão com as bordas da tela
    sf::Vector2f posMinerador = mineradorSprt->getPosition();
    float margem = 5.0f;  // Margem para evitar que o minerador fique preso na borda

    if (posMinerador.x < margem) {
        // Colidiu com a borda esquerda
        Mineradorveloc.x *= -elasticidade;
        Mineradorcords.x = margem;
    } 
    else if (posMinerador.x + mineradorSprt->getGlobalBounds().width > janelaLargura - margem) {
        // Colidiu com a borda direita
        Mineradorveloc.x *= -elasticidade;
        Mineradorcords.x = janelaLargura - margem - mineradorSprt->getGlobalBounds().width;
    }

    if (posMinerador.y < margem) {
        // Colidiu com a borda superior
        Mineradorveloc.y *= -elasticidade;
        Mineradorcords.y = margem;
    } 
    else if (posMinerador.y + mineradorSprt->getGlobalBounds().height > janelaAltura - margem) {
        // Colidiu com a borda inferior
        Mineradorveloc.y *= -elasticidade;
        Mineradorcords.y = janelaAltura - margem - mineradorSprt->getGlobalBounds().height;
    }

    // Atualiza a posição do sprite após o ajuste
    mineradorSprt->setPosition(Mineradorcords);

    // Gradualmente reduzir a rotação para simular desaceleração
    rotacaoVeloc.y *= 0.98f;  // Desacelera a rotação
}
