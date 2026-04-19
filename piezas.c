#include <stdlib.h>
#include <stdio.h>
#include "piezas.h"
#include "main.h"

//Agrega un puntero de pieza a la lista dinamica de enemigos del juego
static void agregar_enemigo(Juego *juego, Pieza *p){
    ListaEnemigos *le = &juego->enemigos;
    if (le->cantidad == le->capacidad){
        le->capacidad = (le->capacidad == 0) ? 8 : le->capacidad *2; 
        le->lista = realloc(le->lista, le->capacidad * sizeof(Pieza *)); //Si la capacidad esta llena, la duplica con realloc

    }
    le->lista[le->cantidad++] = p;
}

//Reserva memoria para una Pieza, la inicializa y la coloca en el tablero
static Pieza *crear_pieza(char tipo, int hp, int x, int y, Tablero *t){
    Pieza *p = malloc(sizeof(Pieza));
    p->tipo = tipo;
    p->hp = hp;
    p->x = x;
    p->y = y;
    p->activo = 1;
    ((Celda *)t->celdas[y][x])->pieza = p;
    return p;
}

//Coloca al Rey y los enemigos del nivel en posiciones aleatorias validas
//Todas las piezas enemigas quedan registradas en juego->enemigos




void spawn_nivel(Juego *juego, int nivel) {
    Tablero *t = juego->t;
    
    //Inicializar lista de enemigos vacia:
    juego->enemigos.lista = NULL;
    juego->enemigos.cantidad = 0;
    juego->enemigos.capacidad = 0;

    //Rey en la fila inferior:
    int rey_x = 1 + rand() % (t->W -2);
    juego->jugador = crear_pieza('R', 10, rey_x, 0, t);

    if(nivel == 1){
        //4 peones en fila 1:
        for (int i = 0; i<4; i++){
            int x = rand() % t->W;
            while (((Celda *)t->celdas[1][x])->pieza != NULL){
                x = rand() % t->W;
            }
            agregar_enemigo(juego, crear_pieza('P', 1, x, t->H - 2, t));
        }

        //2 caballos en fila superior:
        for (int i = 0; i<2; i++){
            int x = rand() % t->W;
            while (((Celda *)t->celdas[t->H -1][x])->pieza != NULL){
                x = rand() % t->W;
            }
            agregar_enemigo(juego, crear_pieza('C',2 ,x ,t->H -1 ,t));
        }

        //2 alfiles en fila superior:
        for (int i = 0; i<2; i++){
            int x = rand() % t->W;
            while (((Celda *)t->celdas[t->H -1][x])->pieza != NULL){
                x = rand() % t->W;
            }
            agregar_enemigo(juego, crear_pieza('A',2 ,x ,t->H -1 ,t));
        }

    } else if(nivel == 2){

        //4 peones:
        for (int i = 0; i<4; i++){
            int x = rand() % t->W;
            while (((Celda *)t->celdas[1][x])->pieza != NULL){
                x = rand() % t->W;
            }
            agregar_enemigo(juego, crear_pieza('P',1 ,x ,t->H - 2 ,t));
        }

        //2 caballos:
        for (int i = 0; i<2; i++){
            int x = rand() % t->W;
            while (((Celda *)t->celdas[t->H -1][x])->pieza != NULL){
                x = rand() % t->W;
            }
            agregar_enemigo(juego, crear_pieza('C',2 ,x ,t->H -1 ,t));
        }

        //2 torres:
        for (int i = 0; i<2; i++){
            int x = rand() % t->W;
            while (((Celda *)t->celdas[t->H -1][x])->pieza != NULL){
                x = rand() % t->W;
            }
            agregar_enemigo(juego, crear_pieza('T',4 ,x ,t->H -1 ,t));
        }
    }else if(nivel == 3){
        //2 peones:
        for (int i = 0; i<2; i++){
            int x = rand() % t->W;
            while (((Celda *)t->celdas[1][x])->pieza != NULL){
                x = rand() % t->W;
            }
            agregar_enemigo(juego, crear_pieza('P',1 ,x ,t->H - 2 ,t));
        }

        //1 reina:
        int x = rand() % t->W;
        agregar_enemigo(juego, crear_pieza('Q', 3, x, t->H -1, t));

        //1 alfil:
        x = (x+1) % t->W;
        agregar_enemigo(juego, crear_pieza('A', 2, x, t->H -1, t));

        //1 torre:
        x = (x+2) % t->W;
        agregar_enemigo(juego, crear_pieza('T', 4, x, t->H -1, t));
    }

    /*
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
    */
}

//Libera cada Pieza enemiga y luego el arreglo de punteros
void lista_enemigos_liberar(ListaEnemigos *le){
    for(int i = 0; i< le->cantidad; i++){
        free(le->lista[i]);
        le->lista[i] = NULL;
    }

    free(le->lista);
    le->lista = NULL;
    le->cantidad = 0;
    le->capacidad = 0;
}

//Mueve cada pieza enemiga viva un paso hacia el Rey segun su tipo
//Implementa el Peon (ortogonal) con deteccion de colision
void mover_enemigos(Juego *juego) {
    Tablero *t = juego->t;
    int rx = juego->jugador->x;
    int ry = juego->jugador->y;

    for(int i = 0; i< juego->enemigos.cantidad; i++){
        Pieza *p = juego->enemigos.lista[i];
        if (!p || p->hp <= 0) continue;

        //limpiar celda actual:
        ((Celda *)t->celdas[p->y][p->x])->pieza = NULL;

        int nx = p->x;
        int ny = p->y;

        if (p->tipo == 'P'){
            // dy apunta hacia el Rey en Y
            int dy = (ry > p->y) ? 1 : -1;

            //El peón se congela si el Rey está en su misma fila o SOBRE el peón
            if (ry == p->y) {
                p->activo = 0;
            } else {
                p->activo = 1;
            }

            if (p->activo) {
                // Ataque diagonal frontal: Rey a 1 casilla diagonal en dirección dy
                if (abs(rx - p->x) == 1 && ry == p->y + dy) {
                    nx = rx;
                    ny = p->y + dy;
                } else {
                    // Avance recto en dirección dy (también cubre ataque frontal directo)
                    ny = p->y + dy;
                }
            }
        }

        else if (p->tipo == 'C'){
            // Los 8 saltos posibles del caballo en forma de L
            int saltos[8][2] = { 
                {-2, -1}, {-2, +1},
                {+2, -1}, {+2, +1},
                {-1, -2}, {+1, -2},
                {-1, +2}, {+1, +2}
            };
            int mejor_x = p->x; // Si no encuentra salto válido, se queda
            int mejor_y = p->y;
            int mejor_dist = abs(rx - p->x) + abs(ry - p->y); 

            for (int s = 0; s < 8; s++){
                int tx = p->x + saltos[s][0];
                int ty = p->y + saltos[s][1];

                // verificamos que estemos dentro del tablero
                if (tx < 0 || tx >= t->W || ty < 0 || ty >= t->H) continue;

                Celda *dest = (Celda *)t->celdas[ty][tx];

                //solo si puede ir a una celda vacia o donde esta el rey
                if (dest->pieza != NULL && dest->pieza != juego->jugador) continue;

                //calcular distancia al rey
                int dist = abs(rx - tx) + abs(ry - ty);

                // Elegir el salto que minimice la distancia
                if (dist < mejor_dist){
                    mejor_dist = dist;
                    mejor_x = tx;
                    mejor_y = ty;
                }
            }
            nx = mejor_x;
            ny = mejor_y;
        }

        else if (p->tipo == 'A'){
            // Las 4 direcciones diagonales
            int diagonales[4][2] = {
                {-1, +1}, {+1, +1},
                {-1, -1}, {+1, -1}
            };

            int mejor_x = p->x; // Si no encuentra movimiento válido, se queda
            int mejor_y = p->y;
            int mejor_dist = abs(rx - p->x) + abs(ry - p->y); // Distancia actual

            for (int d = 0; d < 4; d++){
                int dx = diagonales[d][0];
                int dy = diagonales[d][1];

                //avanza hasta 3 casillas en diagonal
                for (int paso = 1; paso <= 3; paso++){
                    int tx = p->x + dx * paso;
                    int ty = p->y + dy * paso;

                    //verificar limites del tablero
                    if (tx < 0 || tx >= t->W || ty < 0 || ty >= t->H) break;

                    Celda *dest = (Celda *)t->celdas[ty][tx];

                    //si hay una pieza que no sea el rey => el camino está bloqueado
                    if (dest->pieza != NULL && dest->pieza != juego->jugador) break;

                    //calcular distancia
                    int dist = abs(rx - tx) + abs(ry - ty);

                    if (dist < mejor_dist){
                        mejor_dist = dist;
                        mejor_x = tx;
                        mejor_y = ty;
                    }
                    // Si llegó al Rey, parar (no puede seguir más allá)
                    if (dest->pieza == juego->jugador) break;
                }  
            }
            nx = mejor_x;
            ny = mejor_y;
        }




        //AGREGAR AQUI LOS OTROS TIPOS DE PIEZAS
        // REVISAR DSP

        //verificams limites y colision con pieza != al rey:
        if (nx >= 0 && nx < t->W && ny >= 0 && ny < t->H){
            Celda *dest = (Celda *)t->celdas[ny][nx];
            if( dest->pieza == NULL || dest->pieza == juego->jugador){
                p->x = nx;
                p->y = ny;
            }
        }

        ((Celda *)t->celdas[p->y][p->x])->pieza = p;
    }
    /*
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
    */
}

//Comprueba si alguna pieza enemiga ocupa la casilla del Rey
//Retorna false si el Rey fue capturado, true si sigue vivo
bool verificar_estado_rey(Juego *juego) {
    int rx = juego->jugador->x;
    int ry = juego->jugador->y;

    for(int i = 0; i < juego->enemigos.cantidad; i++){
        Pieza *p = juego->enemigos.lista[i];
        if (p && p->hp > 0 && p->x == rx && p->y == ry)
        return false;
    }
    return true;
}