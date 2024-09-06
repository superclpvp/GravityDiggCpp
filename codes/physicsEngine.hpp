#include "Variables.hpp"



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
    
    std::cout<<"normalX: "<<normal.x<<" normalY: "<<normal.y<<"\n";

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

void jogo::fisica(std::vector<std::shared_ptr<Bloco>> blocos, float janelaLargura, float janelaAltura) {
    
    for (auto& min : mineradores) {
        srand(time(0));
        sf::Vector2f aceleracao;
        sf::Vector2f CentroImagem;
        sf::Vector2i cordenadaondeBateu;
        sf::IntRect mineradorRect(min->mineradorSprt->getTextureRect());

        float gravidade = 0.09f;
        float FatorAtrito = 0.05f;

        
        

        min->Mineradorveloc.y += gravidade;

        CentroImagem.x = (mineradorRect.left + mineradorRect.width /2);
        CentroImagem.y = (mineradorRect.top + mineradorRect.height /2);

        min->mineradorSprt->setOrigin(CentroImagem);


        for(auto& bloco : blocos){
            if(Collision::PixelPerfectTest(*min->mineradorSprt,*bloco->blocoSprt)){
                //if(min->Mineradorveloc.y < -0.01f or min->Mineradorveloc.y > 0.01f ){min->parar = true;}
                
            
                cordenadaondeBateu.y = min->Mineradorcords.y;
                cordenadaondeBateu.x = min->Mineradorcords.x;

                std::cout<<bloco->ID<<"\n";

                //std::cout<<"2";//saida para saber se teve alteração
                sf::Vector2f normal = calularNormal(min->mineradorSprt,bloco->blocoSprt);

                
                min->Mineradorveloc.y *= (min->elasticidade*(normal.y));
                min->Mineradorveloc.x *= (min->elasticidade*(normal.x));

                
                float aleatorio = ((rand() % 501) / 1000.f) * ((rand() % 2 == 0) ? 1 : -1);
                
                min->Mineradorveloc.x += aleatorio;
                
                std::cout<<"velocY: "<<min->Mineradorveloc.y<<" velocX: "<<aleatorio<<"\n";
                
            }
            

        }


        min->mineradorSprt->move(min->Mineradorveloc);
    } 
}





