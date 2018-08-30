PREFIX?=/usr
BINDIR?=/bin
SHAREDIR?=/share/geekofall

DATA_PATH?=$(PREFIX)$(SHAREDIR)

CPPFLAGS+=-O2 -Inlohmann/json/single_include -DDATA_PATH=\"$(DATA_PATH)\"
LDFLAGS+=-lsfml-graphics -lsfml-window -lsfml-system -lBox2D -lX11

SOURCES=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SOURCES))

all: geekofall

geekofall: $(OBJ)
	c++ -o geekofall $(LDFLAGS) $(OBJ)

clean:
	rm -f geekofall $(OBJ)

install: geekofall
	install -d geekofall $(DESTDIR)$(PREFIX)$(BINDIR)/geekofall
	install -d geeko.json $(DESTDIR)$(PREFIX)$(SHAREDIR)/geeko.json
	install -d geeko.png  $(DESTDIR)$(PREFIX)$(SHAREDIR)/geeko.png

