#include "engine.hpp"

int main(){
    
    auto joguinho = std::make_shared<jogo>();
    joguinho->run();
 
    return 0;
}