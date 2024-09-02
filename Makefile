# Nome do executável
TARGET = main

# Compilador
CXX = g++

# Diretórios de cabeçalhos e bibliotecas
INCLUDE_DIR = C:/Cpp/bibliotecas/SFML-2.5.1/include
LIB_DIR = C:/Cpp/bibliotecas/SFML-2.5.1/lib

# Opções de compilação e linkedição
CXXFLAGS = -std=c++17 -I$(INCLUDE_DIR) -DSFML_STATIC
LDFLAGS = -L$(LIB_DIR) -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype -lopengl32 -lwinmm -lgdi32 -lsfml-main

# Arquivos fonte e objetos
SRCS = main.cpp codes/bibliotecasExternas/PixelPerfectCollision/Collision.cpp
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