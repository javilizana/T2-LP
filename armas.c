#include <stdio.h>
#include "armas.h"
#include "main.h"

/*
* Nombre: escopeta
* Parámetros: struct Juego *j (puntero al estado del juego), int dir_x 
    (dirección de ataque en el eje x), int dir_y (dirección de ataque en el eje y)
* Retorno: bool (true si el disparo se ejecutó correctamente)
* Descripción: Ejecuta la lógica del disparo de la escopeta. Inflige 2 de daño a la primera
     pieza a 1 casilla de distancia y genera daño en área (1 de daño) en forma de T o 
     cuadrado (dependiendo si el disparo fue ortogonal o diagonal) a las 3 casillas posteriores.
*/
bool escopeta(struct Juego *j, int dir_x, int dir_y){
    int rx = j->jugador->x;
    int ry = j->jugador->y;

    //Para casilla a 1 distancia:
    int cx = rx + dir_x;
    int cy = ry + dir_y;

    //Limpiamos marcas anteriores
    for (int i = 0; i < j->t->H; i++){
        for (int k = 0; k< j->t->W; k++){
            j->marcas[i][k] = 0;
        }
    }

    //impacto directo (2 de daño):
    if (cx >= 0 && cx < j->t->W && cy>= 0 && cy < j->t->H){
        j->marcas[cy][cx] = 1;
        Celda *c = (Celda *)j->t->celdas[cy][cx];
        if (c->pieza && c->pieza->hp > 0){
            c->pieza->hp -=2;
            int hp_mostrar = c->pieza->hp < 0 ? 0 : c->pieza->hp;

            printf("  [Escopeta] Impacto directo en (%d,%d): '%c' recibe 2 de daño (HP: %d/%d)\n", cx, cy, c->pieza->tipo, hp_mostrar, c->pieza->hp_max);
            if (c->pieza->hp <= 0){
                printf("  [Escopeta] '%c' eliminado!\n", c->pieza->tipo);
                c->pieza = NULL; //se elimina la pieza del tablero
            }
        }
    }

    //Para las 3 casillas de atras: se forma una T si el disparo es ortogonal y se forma un cuadrado si es diagonal
    int offsets[3][2];

    if (dir_x != 0 && dir_y != 0){
        //caso diagonal:
        offsets[0][0] = dir_x; offsets[0][1] = 0; 
        offsets[1][0] = 0; offsets[1][1] = dir_y;
        offsets[2][0] = dir_x; offsets[2][1] = dir_y;

        for (int i = 0; i < 3; i++){
            int tx = cx + offsets[i][0];
            int ty = cy + offsets[i][1];

            if(tx < 0 || tx >= j->t->W || ty < 0 || ty >= j->t->H) continue;

            j->marcas[ty][tx] = 1;

            Celda *c = (Celda *)j->t->celdas[ty][tx];
            if(c->pieza && c->pieza->hp > 0){
                c->pieza->hp -= 1;
                int hp_mostrar = c->pieza->hp < 0 ? 0 : c->pieza->hp;
                printf("  [Escopeta] Daño de area en (%d,%d): '%c' recibe 1 de daño (HP: %d/%d)\n", tx, ty, c->pieza->tipo, hp_mostrar, c->pieza->hp_max);
                if(c->pieza->hp <= 0){
                    printf("  [Escopeta] '%c' eliminado!\n", c->pieza->tipo);
                    c->pieza = NULL;
                }
            } 
        }
    }else{ //Caso ortogonal
        int bx = cx + dir_x;
        int by = cy + dir_y;
 
        //Vector perpendicular al disparo
        int perp_x = (dir_x == 0) ? 1 : 0;
        int perp_y = (dir_y == 0) ? 1 : 0;

        offsets[0][0] = -perp_x; 
        offsets[0][1] = -perp_y;
        offsets[1][0] = 0;
        offsets[1][1] = 0;
        offsets[2][0] =  perp_x;
        offsets[2][1] =  perp_y;

        for(int i = 0; i < 3; i++){
            int tx = bx + offsets[i][0];
            int ty = by + offsets[i][1];
 
            if(tx < 0 || tx >= j->t->W || ty < 0 || ty >= j->t->H) continue;
 
            j->marcas[ty][tx] = 1;
            Celda *c = (Celda *)j->t->celdas[ty][tx];
            if(c->pieza && c->pieza->hp > 0){
                c->pieza->hp -= 1;
                int hp_mostrar = c->pieza->hp < 0 ? 0 : c->pieza->hp;
                printf("  [Escopeta] Daño de area en (%d,%d): '%c' recibe 1 de daño (HP: %d/%d)\n", tx, ty, c->pieza->tipo, hp_mostrar, c->pieza->hp_max);
                if(c->pieza->hp <= 0){
                    printf("  [Escopeta] '%c' eliminado!\n", c->pieza->tipo);
                    c->pieza = NULL;
                }
            }
        }

    }
    return true;
}

/*
* Nombre: francotirador
* Parámetros: struct Juego *j (puntero al estado del juego), int dir_x (dirección del disparo 
    en x), int dir_y (dirección del disparo en y)
* Retorno: bool (true si el disparo se ejecutó correctamente)
* Descripción: Dispara en línea recta (ortogonal o diagonal) con alcance infinito. 
    Inflige 3 de daño al primer enemigo que impacta. La bala NO atraviesa a los enemigos.
*/
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

        if(c->pieza && c->pieza->hp > 0){
            j->marcas[cy][cx] = 1; // marca con X en la casilla donde se hizo el impacto
            c->pieza->hp -= 3;
            int hp_mostrar = c->pieza->hp < 0 ? 0 : c->pieza->hp;
            printf("  [Sniper] Impacto en (%d,%d): '%c' recibe 3 de daño (HP: %d/%d)\n", cx, cy, c->pieza->tipo, hp_mostrar, c->pieza->hp_max);
            if(c->pieza->hp <= 0) {
                printf("  [Sniper] '%c' eliminado!\n", c->pieza->tipo);
                c->pieza = NULL;
            }
            break; // No atraviesa: para al primer impacto
        }
        j->marcas[cy][cx] = 2; // marca con - en las casillas de "recorrido"
        cx += dir_x;
        cy += dir_y;
    }
    return true; 
}

/*
* Nombre: granada
* Parámetros: struct Juego *j (puntero al estado del juego), int dir_x (dirección de 
    lanzamiento en x), int dir_y (dirección de lanzamiento en y)
* Retorno: bool (true si se lanzó correctamente, false si la dirección es diagonal o cae 
    fuera del tablero)
* Descripción: Lanza una granada a exactamente 3 casillas de distancia en dirección ortogonal, 
    generando una explosión en un área de 3x3 que inflige 2 de daño a todas las piezas en el 
    radio de impacto.
*/
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
        printf("  [Granada] El lanzamiento cae fuera del tablero :/\n");
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

            // marca las casillas afectadas:
            j->marcas[ty][tx] = 1;

            Celda *c = (Celda *)j->t->celdas[ty][tx];
            if(c->pieza && c->pieza->hp > 0) {
                c->pieza->hp -= 2;
                int hp_mostrar = c->pieza->hp < 0 ? 0 : c->pieza->hp;
                printf("  [Granada] Daño en (%d,%d): '%c' recibe 2 de daño (HP: %d/%d)\n", tx, ty, c->pieza->tipo, hp_mostrar, c->pieza->hp_max);

                if(c->pieza->hp <= 0) {
                    printf("  [Granada] '%c' eliminado!\n", c->pieza->tipo);
                    c->pieza = NULL;
                }
            }  
        }
    }
    return true;
}

/*
* Nombre: especial
* Parámetros: struct Juego *j (puntero al estado del juego), int dir_x (dirección del 
    disparo en x), int dir_y (dirección del disparo en y)
* Retorno: bool (true si el disparo se ejecutó correctamente)
* Descripción: Arma "Sniper 2.0". Dispara en línea recta atravesando a todos los enemigos 
    en su trayectoria. El daño infligido aumenta dependiendo del nivel actual del juego.
*/
bool especial(struct Juego *j, int dir_x, int dir_y) {
    // Limpiar marcas anteriores
    for(int i = 0; i < j->t->H; i++){
        for(int k = 0; k < j->t->W; k++){
            j->marcas[i][k] = 0;
        }
    }

    //int dmg = (j->nivel_actual == 3) ? 2 : j->nivel_actual;
    int dmg  = j->nivel_actual;
    int cx = j->jugador->x + dir_x;
    int cy = j->jugador->y + dir_y;
    int impactos = 0;

    printf("  [Sniper 2.0] Nivel %d: %d de daño por enemigo\n", j->nivel_actual, dmg);

    // Avanza en línea recta atravesando todo hasta salir del tablero
    while(cx >= 0 && cx < j->t->W && cy >= 0 && cy < j->t->H) {
        Celda *c = (Celda *)j->t->celdas[cy][cx];

        if(c->pieza && c->pieza->hp > 0) {
            j->marcas[cy][cx] = 1; // marca con X en la casilla donde se hizo el impacto
            c->pieza->hp -= dmg;
            impactos++;
            int hp_mostrar = c->pieza->hp < 0 ? 0 : c->pieza->hp;
            printf("  [Sniper 2.0] Impacto en (%d,%d): '%c' recibe %d de daño (HP: %d/%d)\n", cx, cy, c->pieza->tipo, dmg, hp_mostrar, c->pieza->hp_max);

            if(c->pieza->hp <= 0) {
                printf("  [Sniper 2.0] '%c' eliminado!\n", c->pieza->tipo);
                c->pieza = NULL;
            }
        }else {
            j->marcas[cy][cx] = 2; // marca con - en las casillas de "recorrido"
        }
        cx += dir_x;
        cy += dir_y;
    }
    
    if(impactos == 0){
        printf("  [Sniper 2.0] No impacto a nadie :c \n");
    }
    return true;
}