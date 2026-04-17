#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"

int main() {
    Juego juego;
    juego.nivel_actual = 1;
    juego.turno_enemigos = 0;
    
    juego.arsenal.municion_actual[0] = 2;
    juego.arsenal.municion_maxima[0] = 2;
    juego.arsenal.disparar[0] = escopeta;
    juego.arsenal.disparar[1] = francotirador;
    juego.arsenal.disparar[2] = granada;
    juego.arsenal.disparar[3] = especial;

    juego.t = tablero_crear(12, 12);
    spawn_nivel(&juego, 1);

    bool jugando = true;
    while(jugando) { // se ejecuta el juego hasta que el jugador decida salir
        tablero_imprimir(&juego);
        printf("Ingrese accion (Tecla de Movimiento o Numero de Arma | 0 Salir): ");
        
        char input;
        scanf(" %c", &input);
        
        if (input == '0') {
            jugando = false;
            continue;
        }

        int prev_x = juego.jugador->x;
        int prev_y = juego.jugador->y;
        bool turno_consumido = false;

        if(input == 'W' || input == 'w') { prev_y++; turno_consumido = true; } // arriba
        else if(input == 'S' || input == 's') { prev_y--; turno_consumido = true; } // abajo
        else if(input == 'A' || input == 'a') { prev_x--; turno_consumido = true; } // izq
        else if(input == 'D' || input == 'd') { prev_x++; turno_consumido = true; } // der
        
        else if(input == 'E' || input == 'e') { prev_x++; prev_y++; turno_consumido = true; } // arriba- der
        else if(input == 'Q' || input == 'q') { prev_x--; prev_y++; turno_consumido = true; } // arriba- izq
        else if(input == 'Z' || input == 'z') { prev_x--; prev_y--; turno_consumido = true; } // abajo- izq
        else if(input == 'C' || input == 'c') { prev_x++; prev_y--; turno_consumido = true; } // abajo- der
        else if(input == '1') {
            printf("Direccion de disparo (Q, W, E, A, S, D, Z, C): ");
            char dir;
            scanf(" %c", &dir);
            int dx = 0, dy = 0;
            if(dir == 'W' || dir == 'w') dy = 1;
            else if(dir == 'S' || dir == 's') dy = -1;
            else if(dir == 'A' || dir == 'a') dx = -1;
            else if(dir == 'D' || dir == 'd') dx = 1;
            
            juego.arsenal.disparar[0](&juego, dx, dy);
            turno_consumido = true;
        }

        if(turno_consumido) {
            if(prev_x >= 0 && prev_x < juego.t->W && prev_y >= 0 && prev_y < juego.t->H) {
                Celda *c_old = (Celda*)juego.t->celdas[juego.jugador->y][juego.jugador->x];
                c_old->pieza = NULL;
                juego.jugador->x = prev_x;
                juego.jugador->y = prev_y;
                Celda *c_new = (Celda*)juego.t->celdas[juego.jugador->y][juego.jugador->x];
                c_new->pieza = juego.jugador;
            }
            mover_enemigos(&juego);
        }
    }

    tablero_liberar(juego.t);
    return 0;
}
