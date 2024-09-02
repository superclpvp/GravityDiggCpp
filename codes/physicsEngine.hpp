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

    if(normal.y == 0){normal.y = 1;}
    if(normal.x == 0){normal.x = 1;}
    
    return normal;
}
float DistanciaAoCentroDeMassa(sf::Vector2f centroDemassa ,std::shared_ptr<sf::Sprite> Sprt1,std::shared_ptr<sf::Sprite> Sprt2){
    sf::Vector2f pos1 = Sprt1->getPosition(); 
    sf::Vector2f pos2 = Sprt2->getPosition(); 
    sf::Vector2f posicaoCentroDeMassa(pos1.x+centroDemassa.x,pos1.y-centroDemassa.y);
    sf::Vector2f VetorSuperficie = pos1 - pos2;

    float DistanciaPontoMassa = std::sqrt(posicaoCentroDeMassa.x*posicaoCentroDeMassa.x - VetorSuperficie.x* VetorSuperficie.x + posicaoCentroDeMassa.y*posicaoCentroDeMassa.y - VetorSuperficie.y* VetorSuperficie.y);

    std::cout<<"DistanciaPontoMassa: "<<DistanciaPontoMassa<<"\n";
    return DistanciaPontoMassa;
}
void minerador::fisica(std::vector<std::shared_ptr<Bloco>> blocos, float janelaLargura, float janelaAltura) {
    
    float forca;
    sf::Vector2f aceleracao;
    sf::Vector2f centroDeGravidade;
    sf::Vector2i cordenadaondeBateu;
    sf::IntRect mineradorRect(mineradorSprt->getTextureRect());
    float torque;

    float gravidade = 0.09f;

    aceleracao.y = gravidade;

    Mineradorveloc.y += aceleracao.y;

    forca = massa * aceleracao.y;

    centroDeGravidade.x = (mineradorRect.left + mineradorRect.width /2);
    centroDeGravidade.y = (mineradorRect.top + mineradorRect.height /2);

    for(auto& bloco : blocos){
        if(Collision::PixelPerfectTest(*mineradorSprt,*bloco->blocoSprt)){
            //Mineradorveloc.y = 0;
            cordenadaondeBateu.y = Mineradorcords.y;
            cordenadaondeBateu.x = Mineradorcords.x;

            //std::cout<<"2";//saida para saber se teve alteração
            sf::Vector2f normal = calularNormal(mineradorSprt,bloco->blocoSprt);
            float distanciaPG = DistanciaAoCentroDeMassa(centroDeGravidade,mineradorSprt,bloco->blocoSprt);
            
            torque = forca * distanciaPG;

            Mineradorveloc.y = Mineradorveloc.y * -elasticidade * (Mineradorveloc.y * normal.y) * normal.y;
            Mineradorveloc.x = Mineradorveloc.x * -elasticidade * (Mineradorveloc.x * normal.x) * normal.x;

            float forcaAtrito = forca * FatorAtrito;

            Mineradorveloc.y = Mineradorveloc.y - (forcaAtrito/massa);
            Mineradorveloc.x = Mineradorveloc.x - (forcaAtrito/massa);
            
            if(Mineradorveloc.x > -0.1 && Mineradorveloc.x < 0.1){Mineradorveloc.x = 0;}
            //std::cout<<"minerador y: "<<Mineradorveloc.y<< " normal y: "<<normal.y<<"\n";
        }

    }

    Mineradorcords += Mineradorveloc;
    mineradorSprt->setPosition(Mineradorcords);
}







