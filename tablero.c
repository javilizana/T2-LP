#include <stdlib.h>
#include <stdio.h>
#include "tablero.h"
#include "piezas.h"
#include "main.h"

//Reserva memoria en el heap para un tablero de ancho x alto celdas
//Usa triple puntero void***: celdas[y][x] apunta a una Celda en el heap
struct Tablero* tablero_crear(int ancho, int alto) {
    Tablero *t = malloc(sizeof(Tablero));
    t->W = ancho;
    t->H = alto;

    //Nivel 1: arreglo de filas (void**):
    t->celdas = malloc(alto * sizeof(void**));

    for(int i = 0; i < alto; i++){

        //Nivel 2: arreglo de columnas (void*) para cada fila:
        t->celdas[i] = malloc(ancho * sizeof(void*));
        for(int j = 0; j < ancho; j++){
            //Nivel 3: cada Celda individualmente en el heap
            Celda *c = malloc(sizeof(Celda));
            c->pieza = NULL;
            t->celdas[i][j] = c;
        }
    }
    return t;
}

//Renderiza el tablero en consola de arriba hacia abajo con HUD
void tablero_imprimir(Juego *juego) {
    Tablero *t = juego->t;

    //Contar enemigos vivos recorriendo la lista
    int enemigos_restantes = 0;
    for (int i = 0; i<juego->enemigos.cantidad; i++){
        Pieza *p = juego->enemigos.lista[i];
        if(p && p->hp > 0) enemigos_restantes++;
    }

    //REVISAR DSP
    /*
    for(int i=0; i< t->H; i++) {
        for (int j = 0; j < t->W; j++){
            Celda *c = (Celda*)t -> celdas[i][j];
            if(c->pieza != NULL && c->pieza->tipo != 'R'){
                enemigos_restantes++;
            } 
        }
    }
    */

    //Dibujar HUD superior:
    printf("\n======================================================\n");
    printf("Nivel: %d | Enemigos restantes: %d\n", juego->nivel_actual, enemigos_restantes);
    printf("Arsenal: [1] Escopeta (%d/%d)  | [2] Sniper (%d/%d)\n", 
           juego->arsenal.municion_actual[0], juego->arsenal.municion_maxima[0],
           juego->arsenal.municion_actual[1], juego->arsenal.municion_maxima[1]);
    printf("         [3] Granada (%d/%d)   | [4] Especial (%d/%d)\n", 
           juego->arsenal.municion_actual[2], juego->arsenal.municion_maxima[2],
           juego->arsenal.municion_actual[3], juego->arsenal.municion_maxima[3]);
    printf("\n======================================================\n");

    //Imprimir tablero (fila H-1 en la parte superior, fila 0 en la inferior):
    for(int i = t->H -1 ; i>=0; i--){
        printf("%2d ", i); //Imprime el n° de fila
        
        for (int j = 0; j < t->W; j++){
            Celda *c = (Celda *)t->celdas[i][j];
            if(c->pieza){
                printf("[%c]", c->pieza->tipo);
            } else if(juego->marcas[i][j] == 1){
                printf("[X]");
            }else{
                printf("[ ]");
            }
        }
        printf("\n");
    }

    //Imprimir el eje x en la pte inferior:
    printf("  ");
    for(int j = 0; j< t->W; j++){
        printf("%2d ", j);
    }
    printf("\n\n");

    //REVISAR DSP CON CUAL QUEDARME
    //Dibujar HUD inferior con controles:
    printf(" Disparo: [1] Escopeta  [2] Sniper  [3] Granada  [4] Especial\n");
    printf(" Mover: [Q][W][E] / [A][ ][D] / [Z][S][C]  |  [0] Salir\n\n");

    /*
    printf("ACCIONES: Disparo: [1] Escopeta [2] Sniper [3] Granada [4] Especial\n");
    printf("Movimiento:\n");
    printf("[Q][W][E]\n");
    printf("[A] R [D]\n");
    printf("[Z][X][C]\n\n");
    */

}

/*
- Libera UNICAMENTE la memoria del tablero: cada Celda, cada fila y el struct Tablero
- NO libera las piezas, esas son responsabilidad del Juego (se liberan desde main antes de 
llamar esta funcion). 
- Por eso se pone c->pieza = NULL antes de liberar, para dejar claro que la celda no es 
dueña de la pieza y evitar doble free accidental
*/
void tablero_liberar(Tablero *tablero) {
    if(!tablero) return;

    for(int i = 0; i < tablero->H; i++){
        for(int j = 0; j < tablero->W; j++){
            Celda *c = (Celda*)tablero->celdas[i][j];

            //Las piezas ya fueron liberadas desde main.
            //onemos NULL para no hacer doble free accidental
            c->pieza = NULL;
            free(c);
        }
        free(tablero->celdas[i]);
    }
    free(tablero->celdas);
    free(tablero);
}