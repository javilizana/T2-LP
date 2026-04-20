#ifndef PIEZAS_H
#define PIEZAS_H

#include <stdbool.h>

struct Juego;

// Representacion de una pieza en el tablero
typedef struct {
    char tipo; // ’P’=Peon, ’C’=Caballo, ’A’=Alfil, ’T’=Torre, ’Q’=Reina, ’R’=Rey 
    int hp;
    int hp_max; //cantidad maxima de HP (AGREGADO)
    int x, y;
} Pieza;

//Una celda del tablero: apunta a la pieza que la ocupa
typedef struct {
    Pieza *pieza; // NULL si la celda esta vacia
} Celda;

typedef struct{
    Pieza **lista;
    int cantidad;
    int capacidad;
}ListaEnemigos;



//Firmas obligatorias — nombres e implementacion no modificables
void spawn_nivel(struct Juego *juego, int nivel);
void mover_enemigos(struct Juego *juego);
bool verificar_estado_rey(struct Juego *juego); // Revisa si el Rey esta en Jaque

//Funcion auxiliar para liberar la lista de enemigos
void lista_enemigos_liberar(ListaEnemigos *le);

#endif