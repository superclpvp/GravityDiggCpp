all:  compile link

compile:
	g++ -c main.cpp -I"C:\Cpp\bibliotecas\SFML-2.5.1\include" -DSFML_STATIC
link:
	g++ main.o -o main -L"C:\Cpp\bibliotecas\SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype -lopengl32 -lwinmm -lgdi32  -lsfml-main
cleam:
	rm -f main *.o