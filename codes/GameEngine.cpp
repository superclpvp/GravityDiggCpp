/*

 _____                 _ _   _               _ _             
|  __ \               (_) | | |             | (_)            
| |  \/_ __ __ ___   ___| |_| |_ _   _    __| |_  __ _  __ _ 
| | __| '__/ _` \ \ / / | __| __| | | |  / _` | |/ _` |/ _` |
| |_\ \ | | (_| |\ V /| | |_| |_| |_| | | (_| | | (_| | (_| |
 \____/_|  \__,_| \_/ |_|\__|\__|\__, |  \__,_|_|\__, |\__, |
                                  __/ |           __/ | __/ |
                                 |___/           |___/ |___/ 

    um jogo desesnvolvido do 0 por um programador inexperiente

*/

#include "Variables.hpp"
#include "box2d/box2d.h"
std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delim)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

//construtor
Engine::Engine()
{
    
    /*#region definir janela*/
    janela = std::make_shared<sf::RenderWindow>(sf::VideoMode(800,600),"gravity digg cpp");
    janela->setFramerateLimit(90);
    camera.setSize(sf::Vector2f(janela->getSize()));
    camera.setCenter(400,300);
    janela->setView(camera);
    /*#endregion*/

    /*#region definir fundo*/
    bgSprite = std::make_shared<sf::Sprite>();
    FundoTxt.loadFromFile("./recursos/textures/Background/BG1.png");
    bgSprite->setTexture(FundoTxt);
    /*#endregion*/

    /*#region definir mundo da fisica*/
    b2Vec2 grav(0,10);
    mundo = std::make_shared<b2World>(grav);
    /*#endregion*/

}

//desenhar elementos na tela
void Engine::draw()
{

    janela->clear(sf::Color::Black);
    janela->draw(*bgSprite);

    for(auto obj : objetos){
        janela->draw(*obj->objSprite);

    }
    

    janela->display();

    
}

//eventos(inputs do usuario)
void Engine::events() {
    sf::Event evento;

    while (janela->pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) {
            janela->close();
        }

        if (evento.type == sf::Event::MouseWheelScrolled) {
            std::cout << "Scroll";
            sf::Vector2f cameraPos = camera.getCenter();
            if (evento.mouseWheelScroll.delta > 0) {
                std::cout << "Scroll Up\n";
                if (cameraPos.y > 300) {
                    camera.move(0, -scrollSpeed);
                }
            } else if (evento.mouseWheelScroll.delta < 0) {
                std::cout << "Scroll Down\n";
                camera.move(0, scrollSpeed);
            }
            janela->setView(camera);
        }

        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::A) {
            criarOjeto("pa_1", 100, 100);
            std::cout << "A\n";
        }
    }
}
//Fisica e afins

void Engine::EngineRUN() {

    

    for(auto obj : objetos) {
        b2Vec2 pos = obj->body->GetPosition();
        float rot = obj->body->GetAngle();
        rot = rot * 180/pi;
        obj->objSprite->setRotation(rot);
        sf::Vector2f posP(pos.x * MPP, pos.y * MPP);
        obj->objSprite->setPosition(posP);
    }

    mundo->Step(deltaTime, 1,1);

}

//função que junta tudo para rodar
void Engine::run()
{
    clock.restart();  
    LastTime = 0;
    float accumulator = 0.0f;
    float physicsInterval = 1.0f / 60.0f; // Intervalo de física

    while(janela->isOpen()){
        sf::Time elapsed = clock.getElapsedTime();
        deltaTime = (elapsed.asSeconds() - LastTime);
        LastTime = elapsed.asSeconds();

        fps = (0.96 / deltaTime);



        
        EngineRUN();
        draw();
        events();
    }
}



void Engine::criaRetangulo(float x, float y, float w, float h, bool dinamico){

    //bodydef
    b2BodyDef bdef;
    if(dinamico){
        bdef.type = b2_dynamicBody;
    }
    else{
        bdef.type = b2_staticBody;
    }
    bdef.position.Set(PPM * x, PPM * y);

    //fixturedef
    b2FixtureDef fdef;
    b2PolygonShape pshape;

    pshape.SetAsBox(0.5f * h * PPM, 0.5f * w * PPM) ;
    fdef.shape = &pshape;

    //body
    b2Body* body = mundo->CreateBody(&bdef) ;
    body->CreateFixture(&fdef);

}

b2Body* Engine::criarPoligono(std::string tipo, bool dinamico, int x, int y) {
    // Abrir o arquivo JSON
    std::ifstream arquivo("./recursos/ObjectsJSON/models.json");
    nlohmann::json dadosJSON;
    arquivo >> dadosJSON;

    // Definir o corpo (body)
    b2BodyDef bdef;
    bdef.type = dinamico ? b2_dynamicBody : b2_staticBody;
    bdef.position.Set(PPM * x, PPM * y);
    b2Body* body = mundo->CreateBody(&bdef);

    // Procurar o tipo de objeto no JSON
    for (const auto& bodydata : dadosJSON["bodydef"]["bodies"]["body"]) {
        if (bodydata["_name"] == tipo) {
            std::cout << "Encontrei o " << tipo << "\n";

            // Pegar dados dos polígonos
            const auto& polygonsData = bodydata["fixtures"]["fixture"]["polygons"]["polygon"];
            for (const auto& polygonStr : polygonsData) {
                std::vector<std::string> coordenadas = split(polygonStr, ',');
                std::vector<b2Vec2> vertices;

                for (size_t i = 0; i < coordenadas.size(); i += 2) {
                    float x = std::stof(coordenadas[i]);
                    float y = std::stof(coordenadas[i + 1]);
                    x *=PPM;
                    y *=PPM;
                    std::cout << "(" << x << ", " << y << ") ";
                    vertices.push_back(b2Vec2(x, y));
                }

                std::cout << "vertices extraidos: ";
                for (const auto& v : vertices) {
                    std::cout << "(" << v.x << ", " << v.y << ") ";
                }
                std::cout << "\n";
                
                // Verificar se há pelo menos 3 vértices
                if (vertices.size() < 3) {
                    std::cerr << "Erro: Um polígono precisa de pelo menos 3 vertices\n";
                    continue;
                }

                // Criar o shape e fixture
                b2PolygonShape polygonShape;
                polygonShape.Set(vertices.data(), vertices.size());

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &polygonShape;
                fixtureDef.density = std::stof(bodydata["fixtures"]["fixture"]["density"].get<std::string>());
                fixtureDef.friction = std::stof(bodydata["fixtures"]["fixture"]["friction"].get<std::string>());
                fixtureDef.restitution = std::stof(bodydata["fixtures"]["fixture"]["restitution"].get<std::string>());

                body->CreateFixture(&fixtureDef);
            }
        }
    }
    return body;
    std::cerr << "poligono criado com sucesso!\n";
}

void Engine::criarOjeto(std::string tipo,float x, float y){

    if (tipo == "pa_1"){
        std::shared_ptr<Objeto> OBJ;
        sf::Vector2f pos(x,y);
        OBJ = std::make_shared<Objeto>(pos);

        OBJ->tipo = tipo;
        OBJ->objTexture.loadFromFile("./recursos/textures/objects/sprite/pa_1.png");
        OBJ->objSprite->setTexture(OBJ->objTexture);
        OBJ->ID = index + 1;
        OBJ->bodyIndex = OBJ->ID -1;

        OBJ->body = criarPoligono(tipo,true,x,y);


        index++;
        objetos.push_back(OBJ);
    }
    else{
        std::cout<< "tipo não encontrado" <<"\n";
    }

}












