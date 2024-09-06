# Nome do executável
TARGET = GravityDigg

# Compilador
CXX = g++

# Diretórios de cabeçalhos e bibliotecas
INCLUDE_DIR = codes/lib/SFML/include codes/lib/box2d/include codes/lib/nlohmann/include
LIB_DIR = codes/lib/SFML/lib codes/lib/box2d/lib

# Opções de compilação e linkedição
CXXFLAGS = -std=c++17 $(addprefix -I,$(INCLUDE_DIR)) -DSFML_STATIC
LDFLAGS = -Lcodes/lib/SFML/lib -Lcodes/lib/box2d/lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype -lopengl32 -lwinmm -lgdi32 -lsfml-main -lbox2d

# Arquivos fonte e objetos 
SRCS = main.cpp codes/gameEngine.cpp codes/lib/PixelPerfectCollision/Collision.cpp 
OBJS = $(SRCS:.cpp=.o)

# Regra padrão
all: $(TARGET) run

# Regra para criar o executável
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Regra para compilar arquivos .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
	del $(TARGET) *.o

# Rodar o executável
run: $(TARGET)
	$(TARGET).exe
