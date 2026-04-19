#include <stdio.h>
#include "armas.h"
#include "main.h"

bool escopeta(struct Juego *j, int dir_x, int dir_y){
    int rx = j->jugador->x;
    int ry = j->jugador->y;

    //Para casilla a 1 distancia:
    int cx = rx + dir_x;
    int cy = ry + dir_y;

    //REVISAR DSP
    //bool impacto_principal = false;

    if (cx >= 0 && cx < j->t->W && cy>= 0 && cy < j->t->H){
        Celda *c = (Celda *)j->t->celdas[cy][cx];
        if (c->pieza && c->pieza->hp > 0){
            c->pieza->hp -=2;
            printf("  [Escopeta] IMPACTO directo en (%d,%d): %c recibe 2 de daño (HP: %d)\n", cx, cy, c->pieza->tipo, c->pieza->hp);
            if (c->pieza->hp <= 0){
                printf("  [Escopeta] %c eliminado!\n", c->pieza->tipo);
                c->pieza = NULL; //se elimina la pieza del tablero
            }
        }
    }

    //Para las 3 casillas de atras (se forma una especie de cono):
    
    //Perpendicular al disparo:
    int perp_x, perp_y;
    if(dir_x == 0){ 
        perp_x = 1; perp_y = 0;
    } else if(dir_y == 0){
        perp_x = 0 ; perp_y = 1;
    } else{
        perp_x = dir_x; perp_y = -dir_y;
    }

    int bx = cx + dir_x;
    int by = cy + dir_y;

    int offsets[3][2] = { //arreglo 2D que almacena las 3 posiciones del cono
        {-perp_x, -perp_y},
        {0,0},
        {perp_x,perp_y}
    };

    for (int i = 0; i< 3; i++){
        int tx = bx + offsets[i][0];
        int ty = by + offsets[i][1];

        if (tx < 0 || tx >= j->t->W || ty < 0 || ty >= j->t->H) continue;

        Celda *c = (Celda *)j->t->celdas[ty][tx];
        if (c->pieza && c->pieza->hp > 0){
            c->pieza->hp -=1;
            printf("  [Escopeta] Daño de area en (%d,%d): %c recibe 1 de daño (HP: %d)\n",tx, ty, c->pieza->tipo, c->pieza->hp);
            
            if (c->pieza->hp <= 0){
                printf("  [Escopeta] %c eliminado!\n", c->pieza->tipo);
                c->pieza = NULL; //se elimina la pieza del tablero
            }
        }
    }

    // Limpiar marcas anteriores
    for(int i = 0; i < j->t->H; i++){
        for(int k = 0; k < j->t->W; k++){
            j->marcas[i][k] = 0;
        }
    }

    // Marcar impacto directo
    if(cx >= 0 && cx < j->t->W && cy >= 0 && cy < j->t->H){
        j->marcas[cy][cx] = 1;
    }

    // Marcar el "cono"
    for(int i = 0; i < 3; i++){
        int tx = bx + offsets[i][0];
        int ty = by + offsets[i][1];
        if(tx >= 0 && tx < j->t->W && ty >= 0 && ty < j->t->H){
            j->marcas[ty][tx] = 1;
        }
    }
    return true;
}

bool francotirador(struct Juego *j, int dir_x, int dir_y) { return true; }
bool granada(struct Juego *j, int target_x, int target_y) { return true; }
bool especial(struct Juego *j, int dir_x, int dir_y) { return true; }