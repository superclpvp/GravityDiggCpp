#include "Variables.hpp"
#include <map>
#include <memory>
#include <unordered_map>
#include <math.h>


/*
    fisica 2
    Colocar o conceito de gravidade(isso é a premissa do jogo)
    colocar o centro de massa mais realista
    fazer um "bounce" mais convicente
    tomar mais café

    vou mudar algumas(muitas(talvez tudo)) variaveis dentro da classe minerador

*/

sf::Vector2f calularNormal(std::shared_ptr<sf::Sprite> Sprt1,std::shared_ptr<sf::Sprite> Sprt2){
    sf::Vector2f pos1 = Sprt1->getPosition(); 
    sf::Vector2f pos2 = Sprt2->getPosition(); 
    
    sf::Vector2f VetorSuperficie = pos1 - pos2;

    float magnitude = std::sqrt((VetorSuperficie.x * VetorSuperficie.x + VetorSuperficie.y * VetorSuperficie.y));

    sf::Vector2 normal = VetorSuperficie / magnitude;

    if(normal.y >= 0){normal.y = 1;} else if (normal.y < 0) {normal.y = -1;}
    if(normal.x >= 0){normal.x = 1;} else if (normal.x < 0) {normal.x = -1;}
    
    return normal;
}
float DistanciaAoCentroDeMassa(sf::Vector2f centroDemassa ,std::shared_ptr<sf::Sprite> Sprt1,std::shared_ptr<sf::Sprite> Sprt2){
    sf::Vector2f pos1 = Sprt1->getPosition(); 
    sf::Vector2f pos2 = Sprt2->getPosition(); 
    sf::Vector2f posicaoCentroDeMassa(pos1.x+centroDemassa.x,pos1.y-centroDemassa.y);
    sf::Vector2f VetorSuperficie = pos1 - pos2;

    float DistanciaPontoMassa = std::sqrt(posicaoCentroDeMassa.x*posicaoCentroDeMassa.x - VetorSuperficie.x* VetorSuperficie.x + posicaoCentroDeMassa.y*posicaoCentroDeMassa.y - VetorSuperficie.y* VetorSuperficie.y);

    //std::cout<<"DistanciaPontoMassa: "<<DistanciaPontoMassa<<"\n";
    return DistanciaPontoMassa;
}

void jogo::fisica(std::vector<std::shared_ptr<Bloco>> blocos, float janelaLargura, float janelaAltura,sf::Time deltaTime) {
    float deltaTimeSeconds = deltaTime.asSeconds();
    for (auto& min : mineradores) {

        sf::Vector2f aceleracao;
        sf::Vector2f centroDeGravidade;
        sf::Vector2i cordenadaondeBateu;
        sf::IntRect mineradorRect(min->mineradorSprt->getTextureRect());


        float forca;
        float torque;
        float resistenciadoAR = 0.01;
        float distanciaPG;
        float AceleracaoAngular;
        float gravidade = 0.09f;
        float velocidadeMaxima = 5;
        float momentoInercia = 0.083 * min->massa * (54*54);

        aceleracao.y = gravidade;

        min->Mineradorveloc.y += aceleracao.y;

        centroDeGravidade.x = (mineradorRect.left + mineradorRect.width /2);
        centroDeGravidade.y = (mineradorRect.top + mineradorRect.height /2);

        min->mineradorSprt->setOrigin(centroDeGravidade);
  
        min->mineradorSprt->setRotation(min->rotacao);

        for(auto& bloco : blocos){
            if(Collision::PixelPerfectTest(*min->mineradorSprt,*bloco->blocoSprt)){
                //if(min->Mineradorveloc.y < -0.01f or min->Mineradorveloc.y > 0.01f ){min->parar = true;}
                
            
                cordenadaondeBateu.y = min->Mineradorcords.y;
                cordenadaondeBateu.x = min->Mineradorcords.x;

                std::cout<<bloco->ID<<"\n";

                //std::cout<<"2";//saida para saber se teve alteração
                sf::Vector2f normal = calularNormal(min->mineradorSprt,bloco->blocoSprt);
                distanciaPG = DistanciaAoCentroDeMassa(centroDeGravidade,min->mineradorSprt,bloco->blocoSprt);
                min->tempo = 0;

            
                min->Mineradorveloc.y *= (1-min->FatorAtrito);
                min->Mineradorveloc.x *= (1-min->FatorAtrito);
                std::cout<<"velocidadeAtritada: "<<min->Mineradorveloc.y<<"\n";

                

                min->Mineradorveloc.y *= -min->elasticidade * (min->Mineradorveloc.y * normal.y) * normal.y;
                min->Mineradorveloc.x *= (-min->elasticidade * (min->Mineradorveloc.x * normal.x) * normal.x);
                std::cout<<"velocidadepos: "<<min->Mineradorveloc.y<<"\n";
                torque = forca * distanciaPG;

                

                AceleracaoAngular = (torque/momentoInercia);
                            
                min->velocidadeAngular -= AceleracaoAngular;
                min->Mineradorveloc.x +=AceleracaoAngular + (0.5 * ((rand() % 2 == 0) ? 1 : -1));
                min->Mineradorveloc.y +=AceleracaoAngular;


                //min->Mineradorveloc.y = 0;
                std::cout<<"dano1 "<<"\n";
                if(min->Mineradorveloc.y < -0.1 or min->Mineradorveloc.y > 0.1){
                    std::cout<<"dano2s "<<"\n";
                    bloco->vida -= min->dano;
                    if (bloco->vida <= 0){
                        min->Mineradorveloc.y *= -1.0f * (min->Mineradorveloc.y * normal.y) * normal.y;
                        min->Mineradorveloc.x *= -1.0f * (min->Mineradorveloc.x * normal.x) * normal.x;
                        std::cout<<"destroi: "<<min->Mineradorveloc.y<<"\n";
                        destruirBlocos(bloco->ID);
                    }
                }

                if(min->Mineradorveloc.x > -0.1 && min->Mineradorveloc.x < 0.1){min->Mineradorveloc.x = 0;}
                if(min->Mineradorveloc.y < -velocidadeMaxima) {min->Mineradorveloc.y = -velocidadeMaxima;} else if(min->Mineradorveloc.y > velocidadeMaxima ){min->Mineradorveloc.y = velocidadeMaxima;}

            
                
            }
            

        }


        forca = min->massa * min->Mineradorveloc.x; 
        //min->rotacao = min->mineradorSprt->getRotation();
        //min->rotacao += (min->velocidadeAngular * deltaTimeSeconds) + 0.5* AceleracaoAngular * (deltaTimeSeconds * deltaTimeSeconds);   
        
        float ValorAbsolutoVelocidadeY = std::abs(min->Mineradorveloc.y);
        min->Mineradorveloc.y *= (1-resistenciadoAR);

        min->mineradorSprt->move(min->Mineradorveloc);
    } 
}





