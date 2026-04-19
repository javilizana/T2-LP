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

bool francotirador(struct Juego *j, int dir_x, int dir_y) { 
    // Limpiar marcas anteriores
    for(int i = 0; i < j->t->H; i++){
        for(int k = 0; k < j->t->W; k++){
            j->marcas[i][k] = 0;
        }
    }
    int cx = j->jugador->x + dir_x;
    int cy = j->jugador->y + dir_y;

    // Avanzar en línea recta hasta salir del tablero
    while(cx >= 0 && cx < j->t->W && cy >= 0 && cy < j->t->H){
        Celda *c = (Celda *)j->t->celdas[cy][cx];

        j->marcas[cy][cx] = 1; // Marcar trayectoria (opcional, visual)

        if(c->pieza && c->pieza->hp > 0){
            c->pieza->hp -= 3;
            printf("  [Sniper] IMPACTO en (%d,%d): %c recibe 3 de daño (HP: %d)\n", cx, cy, c->pieza->tipo, c->pieza->hp);
            if(c->pieza->hp <= 0) {
                printf("  [Sniper] %c eliminado!\n", c->pieza->tipo);
                c->pieza = NULL;
            }
            break; // No atraviesa: para al primer impacto
        }
        cx += dir_x;
        cy += dir_y;
    }
    return true; 
}
bool granada(struct Juego *j, int dir_x, int dir_y) {
    // limpiamos marcas anteriores
    for(int i = 0; i < j->t->H; i++){
        for(int k = 0; k < j->t->W; k++){
            j->marcas[i][k] = 0;
        }
    }
    // rechaza diagonales 
    if(dir_x != 0 && dir_y != 0) {
        printf("  [Granada] Solo se puede lanzar en direcciones ortogonales (W/A/S/D).\n");
        return false;
    }

    // Centro del impacto
    int cx = j->jugador->x + dir_x * 3;
    int cy = j->jugador->y + dir_y * 3;

    // verificamos que el centro caiga dentro del tablero
    if(cx < 0 || cx >= j->t->W || cy < 0 || cy >= j->t->H) {
        printf("  [Granada] El lanzamiento cae fuera del tablero.\n");
        return false;
    }

    printf("  [Granada] Explosion en (%d,%d)!\n", cx, cy);

    // Explotar en area 3x3 centrada en (cx, cy)
    for(int dy = -1; dy <= 1; dy++) {
        for(int dx = -1; dx <= 1; dx++) {
            int tx = cx + dx;
            int ty = cy + dy;

            // verificamos limites del tablero
            if(tx < 0 || tx >= j->t->W || ty < 0 || ty >= j->t->H) continue;

            // marcamos casilla afectada
            j->marcas[ty][tx] = 1;

            Celda *c = (Celda *)j->t->celdas[ty][tx];
            if(c->pieza && c->pieza->hp > 0) {
                c->pieza->hp -= 2;
                printf("  [Granada] Daño en (%d,%d): %c recibe 2 de daño (HP: %d)\n",
                       tx, ty, c->pieza->tipo, c->pieza->hp);

                if(c->pieza->hp <= 0) {
                    printf("  [Granada] %c eliminado!\n", c->pieza->tipo);
                    c->pieza = NULL;
                }
            }  
        }
    }
    return true;
}
bool especial(struct Juego *j, int dir_x, int dir_y) {
    // Limpiar marcas anteriores
    for(int i = 0; i < j->t->H; i++){
        for(int k = 0; k < j->t->W; k++){
            j->marcas[i][k] = 0;
        }
    }

    int dmg = (j->nivel_actual == 3) ? 2 : j->nivel_actual;
    int cx = j->jugador->x + dir_x;
    int cy = j->jugador->y + dir_y;
    int impactos = 0;

    printf("  [Rayo Perforante] Nivel %d: %d de daño por enemigo\n", j->nivel_actual, dmg);

    // Avanza en línea recta atravesando todo hasta salir del tablero
    while(cx >= 0 && cx < j->t->W && cy >= 0 && cy < j->t->H) {
        j->marcas[cy][cx] = 1; // Marcar trayectoria

        Celda *c = (Celda *)j->t->celdas[cy][cx];
        if(c->pieza && c->pieza->hp > 0) {
            c->pieza->hp -= dmg;
            impactos++;
            printf("  [Rayo Perforante] IMPACTO en (%d,%d): %c recibe %d de daño (HP: %d)\n", cx, cy, c->pieza->tipo, dmg, c->pieza->hp);

            if(c->pieza->hp <= 0) {
                printf("  [Rayo Perforante] %c eliminado!\n", c->pieza->tipo);
                c->pieza = NULL;
            }
        }
        cx += dir_x;
        cy += dir_y;
    }
    
    if(impactos == 0){
        printf("  [Rayo Perforante] No impacto a nadie :c \n");
    }
    return true;
}