#include <stdlib.h>
#include "piezas.h"
#include "main.h"

void spawn_nivel(struct Juego *juego, int nivel) {
    Tablero *t = juego->t;
    
    Pieza *rey = malloc(sizeof(Pieza));
    rey->tipo = 'R';
    rey->hp = 10;
    rey->x = t->W / 2;
    rey->y = 0;
    juego->jugador = rey;
    ((Celda*)t->celdas[rey->y][rey->x])->pieza = rey;

    Pieza *peon = malloc(sizeof(Pieza));
    peon->tipo = 'P';
    peon->hp = 1;
    peon->x = t->W / 2;
    peon->y = t->H - 1;
    ((Celda*)t->celdas[peon->y][peon->x])->pieza = peon;
}

void mover_enemigos(struct Juego *juego) {
    Tablero *t = juego->t;
    int rx = juego->jugador->x;
    int ry = juego->jugador->y;

    for(int i = 0; i < t->H; i++){
        for(int j = 0; j < t->W; j++){
            Celda *c = (Celda*)t->celdas[i][j];
            if(c->pieza && c->pieza->tipo == 'P'){
                Pieza *p = c->pieza;
                c->pieza = NULL; 
                
                if(p->y > ry) p->y--;
                else if(p->y < ry) p->y++;
                else if(p->x > rx) p->x--;
                else if(p->x < rx) p->x++;

                ((Celda*)t->celdas[p->y][p->x])->pieza = p;
                return;
            }
        }
    }
}

bool verificar_estado_rey(struct Juego *juego) {
    return true;
}