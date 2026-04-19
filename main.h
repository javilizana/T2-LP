#ifndef MAIN_H
#define MAIN_H

#include "tablero.h"
#include "armas.h"
#include "piezas.h"

typedef struct Juego {
    Tablero *t; //puntero a tablero actual
    Armas arsenal; //armas del jugador
    Pieza *jugador; //puntero a la pieza del jugador (rey)
    ListaEnemigos enemigos;
    int nivel_actual; // variable para saber en que etapa estamos
    int turno_enemigos; // variable para saber en que etapa estamos
    int marcas[12][12]; // 1 = X en esa casilla, 0 = nada (tamaño máximo nivel 1)
} Juego;

#endif