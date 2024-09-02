#include "Variables.hpp"
#include <map>
#include <memory>
#include <unordered_map>
/*
    fisica 2
    Colocar o conceito de gravidade(isso é a premissa do jogo)
    colocar o centro de massa mais realista
    fazer um "bounce" mais convicente
    tomar mais café

    vou mudar algumas(muitas(talvez tudo)) variaveis dentro da classe minerador

*/
void minerador::fisica(std::vector<std::shared_ptr<Bloco>> blocos, float janelaLargura, float janelaAltura) {

    float gravidade = 0.01f;

    Mineradorveloc.y -= gravidade;

    Mineradorcords += Mineradorveloc;
    mineradorSprt->setPosition(Mineradorcords);














}







