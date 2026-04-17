CC=gcc
CFLAGS=-std=c11 -Wall

all: juego

juego: main.o tablero.o armas.o piezas.o
	$(CC) $(CFLAGS) -o juego main.o tablero.o armas.o piezas.o

main.o: main.c main.h
	$(CC) $(CFLAGS) -c main.c

tablero.o: tablero.c tablero.h
	$(CC) $(CFLAGS) -c tablero.c

armas.o: armas.c armas.h
	$(CC) $(CFLAGS) -c armas.c

piezas.o: piezas.c piezas.h
	$(CC) $(CFLAGS) -c piezas.c

clean:
	rm -f *.o juego