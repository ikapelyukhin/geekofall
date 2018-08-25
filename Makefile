all:
	g++ -c main.cpp -I. -I./nlohmann/json/single_include
	g++ main.o -o geekofall -lsfml-graphics -lsfml-window -lsfml-system -lBox2D -lX11
