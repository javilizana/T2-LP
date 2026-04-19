#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> //para usar srand
#include "main.h"

int main(void) {
    srand((unsigned)time(NULL)); //inicializa el aleatorio usando la hora actual como semilla
    // sin esto el juego sería predecible :p

    Juego juego;
    juego.nivel_actual = 1; 
    juego.turno_enemigos = 0;
    juego.jugador = NULL;
    
    for(int i = 0; i < 12; i++){
        for(int k = 0; k < 12; k++){
            juego.marcas[i][k] = 0;
        }
    }

    //Inicializamos arsenal:
    juego.arsenal.municion_actual[0] = 2;
    juego.arsenal.municion_actual[1] = 1;
    juego.arsenal.municion_actual[2] = 2;
    juego.arsenal.municion_actual[3] = 1;

    juego.arsenal.municion_maxima[0] = 2;
    juego.arsenal.municion_maxima[1] = 1;
    juego.arsenal.municion_maxima[2] = 2;
    juego.arsenal.municion_maxima[3] = 1;

    juego.arsenal.disparar[0] = escopeta;
    juego.arsenal.disparar[1] = francotirador;
    juego.arsenal.disparar[2] = granada;
    juego.arsenal.disparar[3] = especial;


    //creamos tablero del nivel 1 (12x12):
    juego.t = tablero_crear(6, 6); 
    spawn_nivel(&juego, 1); 

    bool jugando = true;
    while(jugando) { // se ejecuta el juego hasta que el jugador decida salir (false)
        tablero_imprimir(&juego);
        for(int i = 0; i < juego.t->H; i++){
            for(int k = 0; k < juego.t->W; k++){
                juego.marcas[i][k] = 0;
            }
        }
        printf(" Accion: ");
        
        char input;
        if(scanf(" %c", &input) != 1) break;
        
        if (input == '0') {
            jugando = false;
            continue;
        }

        int nx = juego.jugador->x;
        int ny = juego.jugador->y;
        bool turno_consumido = false;
        bool es_movimiento = false;

        //movimientos del rey:
        if(input == 'W' || input == 'w') { ny++; turno_consumido = es_movimiento = true; } // arriba
        else if(input == 'S' || input == 's') { ny--; turno_consumido = es_movimiento = true; } // abajo
        else if(input == 'A' || input == 'a') { nx--; turno_consumido = es_movimiento = true; } // izq
        else if(input == 'D' || input == 'd') { nx++; turno_consumido = es_movimiento = true; } // der
        
        else if(input == 'E' || input == 'e') { nx++; ny++; turno_consumido = es_movimiento = true; } // arriba- der
        else if(input == 'Q' || input == 'q') { nx--; ny++; turno_consumido = es_movimiento = true; } // arriba- izq
        else if(input == 'Z' || input == 'z') { nx--; ny--; turno_consumido = es_movimiento = true; } // abajo- izq
        else if(input == 'C' || input == 'c') { nx++; ny--; turno_consumido = es_movimiento = true; } // abajo- der
        
        //Disparos:
        else if(input >= '1' && input <= '4') {
            int idx = input - '1';

            if (juego.arsenal.municion_actual[idx] <= 0){
                printf("  Sin municion en arma %d. Turno no consumido.\n", idx + 1);
            } else{
                printf("Direccion de disparo (Q, W, E, A, S, D, Z, C): ");
                char dir;

                if (scanf(" %c", &dir) == 1){
                    int dx = 0, dy = 0;
                    if (dir == 'W' || dir == 'w') {dy =  1;}
                    else if(dir == 'S' || dir == 's'){dy = -1;}
                    else if(dir == 'D' || dir == 'd'){dx = 1;}
                    else if(dir == 'A' || dir == 'a'){dx = -1;}

                    else if(dir == 'Q' || dir == 'q'){dx = -1; dy = 1;}
                    else if(dir == 'E' || dir == 'e'){dx = 1; dy = 1;}
                    else if(dir == 'Z' || dir == 'z'){dx = -1; dy = -1;}
                    else if(dir == 'C' || dir == 'c'){dx = 1; dy = -1;}

                    if (dx != 0 || dy != 0){
                        bool disparo_exitoso = juego.arsenal.disparar[idx](&juego, dx, dy);
                        if (disparo_exitoso){
                            juego.arsenal.municion_actual[idx]--;
                            turno_consumido = true;
                        } 
                    }else {
                        printf("  Direccion invalida. Turno no consumido.\n");
                    }
                }
            }
            
            /*
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
            */
        }

        //Aplicar movimiento del Rey si es valido
        if(es_movimiento) {
            if(nx >= 0 && nx < juego.t->W && ny >= 0 && ny < juego.t->H) {
                ((Celda *)juego.t->celdas[juego.jugador->y][juego.jugador->x])->pieza = NULL;
                juego.jugador->x = nx;
                juego.jugador->y = ny;
                ((Celda *)juego.t->celdas[ny][nx])->pieza = juego.jugador;

                //La escopeta recarga 1 bala por cada movimiento (max 2)
                if (juego.arsenal.municion_actual[0] < juego.arsenal.municion_maxima[0]){
                    juego.arsenal.municion_actual[0]++;
                }
                
            } else {
                printf("  Movimiento fuera del tablero. Turno no consumido.\n");
                turno_consumido = false;
            } 
        }
        //Turno de enemigos:
        if(turno_consumido){
            mover_enemigos(&juego);
            juego.turno_enemigos++;

            if(!verificar_estado_rey(&juego)){
                tablero_imprimir(&juego);
                printf("  *** El Rey ha sido capturado. FIN DEL JUEGO. ***\n");
                jugando = false;
            } else{
                int vivos = 0;
                for(int i = 0; i < juego.enemigos.cantidad; i++){
                    if(juego.enemigos.lista[i] && juego.enemigos.lista[i]->hp > 0){
                        vivos++;
                    }
                }
                // verificamos si todos los enemigos fueron eliminados
                if(vivos == 0){
                    if(juego.nivel_actual == 3){
                        tablero_imprimir(&juego);
                        printf("  *** FELICIDADES! Ganaste el juego! ***\n");
                        jugando = false;
                    } else{
                        juego.nivel_actual++;
                        printf("\n  NIVEL %d COMPLETADO! El Rey avanza...\n", juego.nivel_actual - 1);
                        printf("  Recuperando municion total...\n");

                        for(int i = 0; i < 4; i++){
                            juego.arsenal.municion_actual[i] = juego.arsenal.municion_maxima[i];
                        }
                        lista_enemigos_liberar(&juego.enemigos);
                        if(juego.jugador){ free(juego.jugador); juego.jugador = NULL; }
                        tablero_liberar(juego.t);

                        int nuevo_w, nuevo_h;
                        //Tablero 2do nivel:
                        if(juego.nivel_actual == 2){ 
                            nuevo_w = 8;
                            nuevo_h = 8;
                        } else{ //Tablero 3er nivel:
                            nuevo_w = 6;
                            nuevo_h = 6;
                        }
                        for(int i = 0; i < 12; i++){
                            for(int k = 0; k < 12; k++){
                                juego.marcas[i][k] = 0;
                            }
                        }
                        juego.t = tablero_crear(nuevo_w, nuevo_h);
                        juego.turno_enemigos = 0;
                        spawn_nivel(&juego, juego.nivel_actual);
                    }
                }
            }
        }
    }

    /*
    Liberacion de memoria en el orden:
    1. Piezas enemigas (lista_enemigos_liberar)
    2. Pieza del Rey (free directo) 
    3. Tablero (tablero_liberar con firma original)
    */
    
    lista_enemigos_liberar(&juego.enemigos);
    if (juego.jugador){
        free(juego.jugador);
        juego.jugador = NULL;
    }
    tablero_liberar(juego.t);
    return 0;
}
