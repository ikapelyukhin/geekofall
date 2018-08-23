all:
	g++ -c main.cpp 
	g++ main.o -o geekofall -lsfml-graphics -lsfml-window -lsfml-system -lBox2D

