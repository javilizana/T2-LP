#include <stdlib.h>
#include <stdio.h>
#include "tablero.h"
#include "piezas.h"
#include "main.h"

struct Tablero* tablero_crear(int ancho, int alto) {
    Tablero *t = malloc(sizeof(Tablero));
    t->W = ancho;
    t->H = alto;
    t->celdas = malloc(alto * sizeof(void**));
    for(int i = 0; i < alto; i++){
        t->celdas[i] = malloc(ancho * sizeof(void*));
        for(int j = 0; j < ancho; j++){
            Celda *c = malloc(sizeof(Celda));
            c->pieza = NULL;
            t->celdas[i][j] = c;
        }
    }
    return t;
}

void tablero_imprimir(struct Juego *juego) {
    Tablero *t = juego->t;
    int enemigos_restantes = 0;
    
    

    for(int i=0; i< t->H; i++) {
        for (int j = 0; j < t->W; j++){
            Celda *c = (Celda*)t -> celdas[i][j];
            if(c->pieza != NULL && c->pieza->tipo != 'R'){
                enemigos_restantes++;
            } 
        }
    }

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

    //Imprimir tablero:
    for(int i = t->H -1 ; i>=0; i--){
        printf("%d ", i); //Imprime el n° de fila
        for (int j = 0; j < t->W; j++){
            Celda *c = (Celda*)t -> celdas[i][j];
            if(c->pieza != NULL){
                printf("[%c]", c->pieza->tipo);
            } else{
                printf("[ ]");
            }
        }
        printf("\n");
    }

    //Imprimir el eje x en la pte inferior:
    printf("  ");
    for(int j = 0; j< t->W; j++){
        printf(" %d ", j);
    }
    printf("\n\n");

    //Dibujar HUD inferior:
    printf("ACCIONES: Disparo: [1] Escopeta [2] Sniper [3] Granada [4] Especial\n");
    printf("Movimiento:\n");
    printf("[Q][W][E]\n");
    printf("[A] R [D]\n");
    printf("[Z][X][C]\n\n");

    



}

void tablero_liberar(struct Tablero *tablero) {
    if(!tablero) return;
    for(int i = 0; i < tablero->H; i++){
        for(int j = 0; j < tablero->W; j++){
            Celda *c = (Celda*)tablero->celdas[i][j];
            if(c->pieza) free(c->pieza);
            free(c);
        }
        free(tablero->celdas[i]);
    }
    free(tablero->celdas);
    free(tablero);
}