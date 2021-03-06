CC=gcc

CFLAGS=-g -Wall -D_GNU_SOURCE=1 -D_REENTRANT
#CFLAGS=-g -Wall -D_REENTRANT
IMAGE_FLAGS=-lSDL2_image
AUDIO_FLAGS =-lSDL2_mixer
SFLAGS=-lSDL2
SOURCES=breakout.c
BINARIES=breakout

#all: ex0 ex1 ex2 ex3 ex4 ex5 ex6 ex7 grafico texto
all: $(BINARIES)

06_moving_images: 06_moving_images.c
	$(CC) -o breakout breakout.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(AUDIO_FLAGS)

clean:
	rm -rf *.o *.exe *.bak *.c~ $(BINARIES) core a.out
