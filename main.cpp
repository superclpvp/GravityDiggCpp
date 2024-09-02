#include "codes/GameEngine.hpp"

int main(){
    
    auto joguinho = std::make_shared<jogo>();
    joguinho->run();
 
    return 0;
}