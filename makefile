CC=gcc

CFLAGS=-g -Wall -D_GNU_SOURCE=1 -D_REENTRANT -std=c90 -pedantic
#CFLAGS=-g -Wall -D_REENTRANT
SFLAGS=-lSDL2
IMAGE_FLAGS=-lSDL2_image
MIXER_FLAGS=-lSDL2_mixer
TTF_FLAGS=-lSDL2_ttf
SOURCES=main.c funcoes.c Globais.c
OBJECTS=main.o funcoes.o Globais.o
BINARIES=breakout

all: $(BINARIES)

breakout: main.o funcoes.o Globais.o
	$(CC) -o breakout main.o funcoes.o Globais.o $(SFLAGS) $(IMAGE_FLAGS) $(MIXER_FLAGS) $(TTF_FLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(MIXER_FLAGS) $(TTF_FLAGS)

funcoes.o: funcoes.c
	$(CC) -c funcoes.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(MIXER_FLAGS) $(TTF_FLAGS)
	
Globais.o: Globais.c
	$(CC) -c Globais.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(MIXER_FLAGS) $(TTF_FLAGS)
	
clean:
	rm -rf *.o *.exe *.bak *.c~ $(BINARIES) core a.out

limpa:
	del *.o *.exe *.bak *.c~ $(BINARIES) core
