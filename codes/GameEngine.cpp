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
    cameraDesc = camera.getCenter().y;
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
    MatrizTerreno =  std::vector<std::vector<int>>(altura, std::vector<int>(larg, 0));
    

    mundo->SetContactListener(&meuContactListener);

}

//desenhar elementos na tela
void Engine::draw()
{

    janela->clear(sf::Color::Black);
    janela->draw(*bgSprite);


    for (auto obj : objetos) {
        janela->draw(*obj->objSprite);
        if(desenharFisica){
        // Itera sobre os fixtures do corpo
        for (b2Fixture* fixture = obj->body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            const b2Shape* shape = fixture->GetShape();
            if (shape->GetType() == b2Shape::e_polygon) {
                const b2PolygonShape* polygonShape = dynamic_cast<const b2PolygonShape*>(shape);
                if (polygonShape) {
                    drawPolygon(*polygonShape, obj->body);
                } else {
                    std::cout << "Failed to cast shape to b2PolygonShape\n";
                }
            }
        }
        }
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
                if (cameraPos.y < maxDown) {
                    camera.move(0, scrollSpeed);
                }
            }
            janela->setView(camera);
            std::cout<<"pos " << cameraPos.y  <<"\n"; 
        }
        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Left) {
            camera.move(32,0);
            janela->setView(camera);
        }
        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Right) {
            camera.move(-32,0);
            janela->setView(camera);
        }

        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::A) {
            criarOjeto("pa_1", 100, 100);
            std::cout << "A\n";
        }
        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::F && !desenharFisica) {
            desenharFisica = true;
        }
        else if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::F && desenharFisica) {
            desenharFisica = false;
        }

        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::C && !travarCamera) {
            travarCamera = true;
        }
        else if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::C && travarCamera) {
            travarCamera = false;
        }
        
    }
}

//Fisica e afins
void Engine::EngineRUN() {

    for(auto obj : objetos) {
        
        b2Vec2 pos = obj->body->GetPosition();
        float rot = obj->body->GetAngle();
        sf::FloatRect bounds = obj->objSprite->getLocalBounds();
        obj->objSprite->setOrigin(bounds.width/2,bounds.height/2);
        rot = (rot * 180/pi) + 180;
        obj->objSprite->setRotation(rot);
        sf::Vector2f posP(pos.x * MPP, pos.y * MPP);
        obj->objSprite->setPosition(posP);

        std::vector<int> idsParaRemover;

        //colisão da pa no bloco
        if(obj->tipo == "pa_1"){
            for (const auto& contato : meuContactListener.contatosAtuais) {
                if (contato.first == obj->body && obj->tipo == "pa_1") {
                    for (auto obj2 : objetos) {
                        if (obj2->body == contato.second) {
                            if (obj2->grupo == "destruitivel") {
                                obj2->Vida -= obj->Dano;
                                std::cout<< "-1" <<"\n";
                            }
                        }
                    }
                } else if (contato.second == obj->body && obj->tipo == "pa_1") {
                    for (auto obj2 : objetos) {
                        if (obj2->body == contato.first) {
                            if (obj2->grupo == "destruitivel") {
                                obj2->Vida -= obj->Dano;
                                std::cout<< "-1" <<"\n";
                            }
                        }
                    }
                }
            }
        }


        //camera do jogo "vai da merda isso aq"
        if(travarCamera && obj->objSprite->getPosition().y > camera.getCenter().y && obj->tipo == "pa_1" &&  obj->objSprite->getPosition().y > 620 ){
            if(obj->objSprite->getPosition().y > cameraDesc){
                cameraDesc = obj->objSprite->getPosition().y - 200;
                camera.move(0,cameraDesc - camera.getCenter().y);
                janela->setView(camera);
                cameraDesc = camera.getCenter().y;
            }
        }
    
    
    }
    destruirObjetos();
    mundo->Step(deltaTime, 1,1);

}

//gerar o terreno base
void Engine::gerarTerreno(){
    std::shared_ptr<sf::Vector2f> ponteiro = std::make_shared<sf::Vector2f>();
    srand(static_cast<unsigned>(time(0))); 
    float limiar = 0.1f;
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.1f);
    noise.SetSeed(seed);


    for (int y = 0; y < altura; y++) {  // y como linha
        for (int x = 0; x < larg; x++) {  // x como coluna
            if (y < 3) {
                MatrizTerreno[y][x] = 0;  // Aqui y é linha e x é coluna
            }
            else if(y < 6){

                // Gerar um número aleatório entre 0 e 100
                int random = 0 + rand() % (100 - 0 + 1);

                if(random > 30){
                    MatrizTerreno[y][x] = 0;
                }
                else{
                    MatrizTerreno[y][x] = 1;
                }
                std::cout<<random<< " ";
            }
            else if( y < 10){
                int random = 0 + rand() % (100 - 0 + 1);

                if(random > 30){
                    MatrizTerreno[y][x] = 1;
                }
                else{
                    MatrizTerreno[y][x] = 0;
                }
            }
            else{

                float valor = noise.GetNoise((float)x, (float)y);

                // Aplica o limiar para definir se é uma parede ou um espaço vazio
                if (valor > limiar) {
                    MatrizTerreno[y][x] = -1;  // Espaço vazio
                } else {
                    MatrizTerreno[y][x] = 1;  // parede
                }

            }
            //std::cout<<MatrizTerreno[y][x];
        }
        //std::cout<<"\n";
    }

    ponteiro->x = -320;
    ponteiro->y = -32;
    for(int i = 0; i<14 ;i++){
        ponteiro->x = 32 + (64*i);
        criarOjeto("Blocker",ponteiro->x,ponteiro->y);
    }
    for(int y = 0; y< 10; y++){
        ponteiro->x = -32;
        criarOjeto("Blocker",ponteiro->x,ponteiro->y);
        ponteiro->x = 832;
        criarOjeto("Blocker",ponteiro->x,ponteiro->y);
        ponteiro->y = 0 + (64*y);
    }

    ponteiro->y = 280;

    for(int y = 0; y< altura; y++){
        ponteiro->x = -32;
        criarOjeto("Blocker",ponteiro->x,ponteiro->y);
        for(int x = 0; x < larg; x++){
            ponteiro->y = 215 + (64*y);
            ponteiro->x = 32 + (64*x);
            if(MatrizTerreno[y][x] == 0){
                criarOjeto("terra",ponteiro->x,ponteiro->y);
            }if(MatrizTerreno[y][x] == 1){
                criarOjeto("pedra",ponteiro->x,ponteiro->y);
            }

        }
        ponteiro->x = 832;
        criarOjeto("Blocker",ponteiro->x,ponteiro->y);
    }
    
}

//função que junta tudo para rodar
void Engine::run()
{
    gerarTerreno();
    clock.restart();  
    LastTime = 0;
    float accumulator = 0.0f;
    float physicsInterval = 1.0f / 60.0f; // Intervalo de física

    while(janela->isOpen()){
        sf::Time elapsed = clock.getElapsedTime();
        deltaTime = (elapsed.asSeconds() - LastTime);
        LastTime = elapsed.asSeconds();

        fps = (0.96 / deltaTime);


        //std::cout<<"fps " << fps<<"\n"; 
        
        EngineRUN();
        draw();
        events();
    }
}



b2Body* Engine::criaRetangulo(float x, float y, float w, float h, bool dinamico){

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
    fdef.density = 10;


    pshape.SetAsBox(0.5f * h * PPM, 0.5f * w * PPM) ;
    fdef.shape = &pshape;

    //body
    b2Body* body = mundo->CreateBody(&bdef) ;
    body->CreateFixture(&fdef);
    return body;

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
                    x *= PPM;
                    y *= PPM;
                    //std::cout << "(" << x << ", " << y << ") ";
                    vertices.push_back(b2Vec2(x, y));
                }

                std::cout << "vertices extraidos: ";
                for (const auto& v : vertices) {
                    //std::cout << "(" << v.x << ", " << v.y << ") ";
                }
                //std::cout << "\n";
                
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
        OBJ->Vida = 10;
        OBJ->Dano = 1;
        OBJ->objTexture.loadFromFile("./recursos/textures/objects/sprite/pa_1.png");
        OBJ->objSprite->setTexture(OBJ->objTexture);
        OBJ->ID = index + 1;
        OBJ->bodyIndex = OBJ->ID -1;

        OBJ->body = criarPoligono(tipo,true,x,y);

        index++;
        objetos.push_back(OBJ);
    }

    else if (tipo == "terra")
    {
        std::shared_ptr<Objeto> OBJ;
        sf::Vector2f pos(x,y);
        OBJ = std::make_shared<Objeto>(pos);

        OBJ->tipo = tipo;
        OBJ->Vida = 3;
        OBJ->grupo = "destruitivel";
        OBJ->objTexture.loadFromFile("./recursos/textures/objects/sprite/terra.png");
        OBJ->objSprite->setTexture(OBJ->objTexture);
        OBJ->ID = index + 1;
        OBJ->bodyIndex = OBJ->ID -1;

        //OBJ->body = criarPoligono(tipo,false,x,y);
        OBJ->body = criaRetangulo(x,y,64,64,false);


        index++;
        objetos.push_back(OBJ);
    }

    else if (tipo == "Blocker")
    {
        std::shared_ptr<Objeto> OBJ;
        sf::Vector2f pos(x,y);
        OBJ = std::make_shared<Objeto>(pos);

        OBJ->tipo = tipo;
        OBJ->objTexture.loadFromFile("./recursos/textures/objects/sprite/Blocker.png");
        OBJ->objSprite->setTexture(OBJ->objTexture);
        OBJ->ID = index + 1;
        OBJ->bodyIndex = OBJ->ID -1;

        //OBJ->body = criarPoligono(tipo,false,x,y);
        OBJ->body = criaRetangulo(x,y,64,64,false);

        index++;
        objetos.push_back(OBJ);
    }

    else if (tipo == "pedra")
    {
        std::shared_ptr<Objeto> OBJ;
        sf::Vector2f pos(x,y);
        OBJ = std::make_shared<Objeto>(pos);

        OBJ->tipo = tipo;
        OBJ->Vida = 10;
        OBJ->grupo = "destruitivel";
        OBJ->objTexture.loadFromFile("./recursos/textures/objects/sprite/pedra.png");
        OBJ->objSprite->setTexture(OBJ->objTexture);
        OBJ->ID = index + 1;
        OBJ->bodyIndex = OBJ->ID -1;

        //OBJ->body = criarPoligono(tipo,false,x,y);
        OBJ->body = criaRetangulo(x,y,64,64,false);

        index++;
        objetos.push_back(OBJ);
    }
    
    else{
        std::cout<< "tipo não encontrado" <<"\n";
    }

}

void Engine::drawPolygon(const b2PolygonShape& shape, const b2Body* body) {
    sf::VertexArray outline(sf::LineStrip);

    // Itera sobre os vértices do polígono, sem adicionar o último vértice
    for (int32 i = 0; i < shape.m_count; ++i) {
        // Pega o vértice local do shape e transforma para posição global
        b2Vec2 worldVertex = body->GetWorldPoint(shape.m_vertices[i]);

        // Converte as coordenadas de Box2D (metros) para SFML (pixels)
        sf::Vector2f position(worldVertex.x * MPP, worldVertex.y * MPP);

        // Adiciona o vértice ao contorno
        outline.append(sf::Vertex(position, sf::Color::Red)); // Cor do contorno
    }

    // Desenha o contorno na janela sem adicionar o primeiro vértice novamente
    janela->draw(outline);
}

void Engine::destruirObjetos() {
    // Encontre o objeto com o ID especificado

    
    
    for(auto obj : objetos){
        if(obj->Vida <= 0 && obj->grupo == "destruitivel"){
            int id = obj->ID;
            auto it = std::find_if(objetos.begin(), objetos.end(), [id](const std::shared_ptr<Objeto>& obj) {
                return obj->ID == id;
            });

            if (obj->body) {
                mundo->DestroyBody(obj->body);
            }

            objetos.erase(it);
            break;


        }
        
        
        
        //Se destruir verdadeiro
        if(obj->destruir == true){
            
            int id = obj->ID;
            auto it = std::find_if(objetos.begin(), objetos.end(), [id](const std::shared_ptr<Objeto>& obj) {
                return obj->ID == id;
            });

            if (obj->body) {
                mundo->DestroyBody(obj->body);
            }

            objetos.erase(it);
            break;
        }
    }

}
