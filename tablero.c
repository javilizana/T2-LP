#include <stdlib.h>
#include <stdio.h>
#include "tablero.h"
#include "piezas.h"
#include "main.h"

/*
* Nombre: tablero_crear
* Parámetros: int ancho (número de columnas del tablero), int alto (número de filas del tablero)
* Retorno: struct Tablero* (puntero a la estructura del tablero generado)
* Descripción: Reserva la memoria dinámica para el tablero bidimensional mediante un 
    triple puntero (void ***) organizándolo en filas, columnas y asignando memoria a cada 
    Celda individual.
*/
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

/*
* Nombre: tablero_imprimir
* Parámetros: Juego *juego (puntero al estado del juego)
* Retorno: void (ninguno)
* Descripción: Dibuja la interfaz visual (HUD) y el tablero actual en consola. Muestra las piezas, 
    las marcas de ataque, balas disponibles, HP, información del nivel y las opciones de acciones permitidas.
*/
void tablero_imprimir(Juego *juego) {
    Tablero *t = juego->t;

    //Contar enemigos vivos recorriendo la lista
    int enemigos_restantes = 0;
    for (int i = 0; i<juego->enemigos.cantidad; i++){
        Pieza *p = juego->enemigos.lista[i];
        if(p && p->hp > 0) enemigos_restantes++;
    }

    //Dibujar HUD superior:
    printf("\n======================================================\n");
    printf("Nivel: %d | Enemigos restantes: %d\n", juego->nivel_actual, enemigos_restantes);
    printf("Arsenal: [1] Escopeta (%d/%d)  | [2] Sniper (%d/%d)\n", 
           juego->arsenal.municion_actual[0], juego->arsenal.municion_maxima[0],
           juego->arsenal.municion_actual[1], juego->arsenal.municion_maxima[1]);
    printf("         [3] Granada (%d/%d)   | [4] Sniper 2.0 (%d/%d)\n", 
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
            } 
            else if (juego->marcas[i][j] == 1){ 
                printf("[X]");
            } else if(juego->marcas[i][j] == 2){
                printf("[-]");
            } else{
                printf("[ ]");
            } 
        }
        printf("\n");
    }

    //Imprime el n° de columna:
    printf("   ");
    for(int j = 0; j< t->W; j++){
        printf("%2d ", j);
    }
    printf("\n\n");

    //Dibujar HUD inferior con controles:
    printf(" Disparo: [1] Escopeta  [2] Sniper  [3] Granada  [4] Snipper 2.0\n");
    printf(" Mover: [Q][W][E] / [A][ ][D] / [Z][S][C]  |  [0] Salir\n\n");

}

/*
* Nombre: tablero_liberar
* Parámetros: Tablero *tablero (puntero al tablero que se desea liberar)
* Retorno: void (ninguno)
* Descripción: Libera de manera segura toda la memoria heap utilizada estrictamente por la 
    estructura Tablero y sus Celdas bidimensionales, sin afectar directamente la memoria de 
    las piezas para evitar conflictos (doble free).
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