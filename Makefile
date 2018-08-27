ifdef DESTDIR
JSON_PATH=-DJSON_PATH='"$(DESTDIR)/geeko.json"'
TEXTURE_PATH=-DTEXTURE_PATH='"$(DESTDIR)/geeko.png"'
endif

CFLAGS=-c main.cpp -I. -I./nlohmann/json/single_include $(JSON_PATH) $(TEXTURE_PATH)
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lBox2D -lX11

all:
	g++ $(CFLAGS)
	g++ main.o -o geekofall $(LDFLAGS)
