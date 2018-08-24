all:
	g++ -c main.cpp -I.
	g++ main.o -o geekofall -lsfml-graphics -lsfml-window -lsfml-system -lBox2D

