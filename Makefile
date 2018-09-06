PREFIX?=/usr
BINDIR?=/bin
SHAREDIR?=/share/geekofall

DATA_PATH?=$(PREFIX)$(SHAREDIR)

CPPFLAGS+=-DDATA_PATH=\"$(DATA_PATH)\" -Wall
LDFLAGS+=-lsfml-graphics -lsfml-window -lsfml-system -lBox2D -lX11

SOURCES=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SOURCES))

all: geekofall

geekofall: $(OBJ)
	c++ -o geekofall $(OBJ) $(LDFLAGS)

clean:
	rm -f geekofall $(OBJ)

install: geekofall
	install -d $(DESTDIR)$(PREFIX)$(BINDIR)
	install -d $(DESTDIR)$(PREFIX)$(SHAREDIR)
	install geekofall $(DESTDIR)$(PREFIX)$(BINDIR)/geekofall
	install geeko.json $(DESTDIR)$(PREFIX)$(SHAREDIR)/geeko.json
	install geeko.png  $(DESTDIR)$(PREFIX)$(SHAREDIR)/geeko.png
