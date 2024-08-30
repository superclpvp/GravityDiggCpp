#include "Variables.hpp"
#include <map>
#include <memory>
#include <unordered_map>

void minerador::fisica(std::vector<std::shared_ptr<Bloco>> blocos, float janelaLargura, float janelaAltura) {
    // Variáveis de controle
    float rotacaoVelocidade = 0.3f;
    float gravidade = 0.005f;
    float reducaoElasticidade = 0.7f;
    float fatorDesaceleracao = 0.99f;
    float velocidadeMinima = 0.01f;

    // Suavizar aceleração e velocidade
    Mineradorveloc.y += gravidade;

    // Suavização do movimento
    Mineradorveloc.x *= fatorDesaceleracao;
    Mineradorveloc.y *= fatorDesaceleracao;

    // Parar movimento se a velocidade for muito pequena
    //if (std::abs(Mineradorveloc.x) < velocidadeMinima) {
    //   Mineradorveloc.x = 0;
    //}
    //if (std::abs(Mineradorveloc.y) < velocidadeMinima) {
    //    Mineradorveloc.y = 0;
    //}

    // Calcular centro de massa
    sf::FloatRect bounds = mineradorSprt->getGlobalBounds();
    sf::Vector2f centroDeMassa = sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);

    // Atualizar posição do minerador
    Mineradorcords += Mineradorveloc;
    mineradorSprt->setPosition(Mineradorcords);

    // Verificar colisão com blocos usando um método mais preciso
    for (auto& bloco : blocos) {
        if (verificarColisaoPrecisa(*mineradorSprt, *bloco->blocoSprt)) {
            // Aqui entra a lógica de quique e ajuste de posição baseada no centro de massa
            if (Mineradorveloc.y > velocidadeMinima) {
                Mineradorveloc.y *= -((elasticidade)*perdaDeEnergia);
                Mineradorveloc.x += ((rand() % 51 - 25) * 0.01f) * perdaDeEnergia;

                std::cout<<Mineradorveloc.y<<"\n";
                
                Mineradorcords += Mineradorveloc;
                if(perdaDeEnergia > 0) {perdaDeEnergia -= 0.01;}
                mineradorSprt->setPosition(Mineradorcords);
            }
            else{
                Mineradorveloc = {0,-gravidade};
                Mineradorcords += Mineradorveloc;
                mineradorSprt->setPosition(Mineradorcords);
                perdaDeEnergia = 1;
            }
            if (Mineradorveloc.x != 0) {
                Mineradorveloc.x *= -elasticidade;
            }
        }
    }

    // Verificar colisão com bordas da tela
    verificarColisaoBordas(Mineradorcords, janelaLargura, janelaAltura);

    // Atualizar posição após ajuste
    mineradorSprt->setPosition(Mineradorcords);

    // Rotação baseada na velocidade
    mineradorSprt->rotate(Mineradorveloc.x * rotacaoVelocidade);

    // Gradualmente reduzir rotação para simular desaceleração
    rotacaoVeloc.y *= fatorDesaceleracao;
}




// Função auxiliar para verificar colisão pixel a pixel ou com máscara
bool minerador::verificarColisaoPrecisa(const sf::Sprite minerador, const sf::Sprite bloco) {
    sf::FloatRect boundsMinerador = minerador.getGlobalBounds();
    sf::FloatRect boundsBloco = bloco.getGlobalBounds();

    // Colisão por AABB (bounding box)
    if (boundsMinerador.intersects(boundsBloco)) {
        // Aqui entraria a lógica para verificar colisão precisa, por exemplo, usando uma máscara de colisão
        // ou verificando se os pixels não transparentes se sobrepõem
        return true;  // Suponha que há colisão como exemplo
    }

    return false;
}

void minerador::verificarColisaoBordas(sf::Vector2f posMinerador, float janelaLargura, float janelaAltura) {
    float margem = 5.0f;

    if (posMinerador.x < margem) {
        Mineradorveloc.x *= -elasticidade;
        Mineradorcords.x = margem;
    } 
    else if (posMinerador.x + mineradorSprt->getGlobalBounds().width > janelaLargura - margem) {
        Mineradorveloc.x *= -elasticidade;
        Mineradorcords.x = janelaLargura - margem - mineradorSprt->getGlobalBounds().width;
    }

    if (posMinerador.y < margem) {
        Mineradorveloc.y *= -elasticidade;
        Mineradorcords.y = margem;
    } 
    else if (posMinerador.y + mineradorSprt->getGlobalBounds().height > janelaAltura - margem) {
        Mineradorveloc.y *= -elasticidade;
        Mineradorcords.y = janelaAltura - margem - mineradorSprt->getGlobalBounds().height;
    }
}









