#include <stdlib.h>
#include <stdio.h>
#include "piezas.h"
#include "main.h"

/*
* Nombre: agregar_enemigo
* Parámetros: Juego *juego (puntero al estado del juego), Pieza *p (puntero a la pieza 
    enemiga creada)
* Retorno: void (ninguno)
* Descripción: Añade una nueva pieza enemiga a la lista dinámica de enemigos del juego. Si la 
    capacidad de la lista se agota, duplica su tamaño utilizando realloc.
*/
static void agregar_enemigo(Juego *juego, Pieza *p){
    ListaEnemigos *le = &juego->enemigos;
    if (le->cantidad == le->capacidad){
        le->capacidad = (le->capacidad == 0) ? 8 : le->capacidad *2; 
        le->lista = realloc(le->lista, le->capacidad * sizeof(Pieza *)); //Si la capacidad esta llena, la duplica con realloc

    }
    le->lista[le->cantidad++] = p;
}

/*
* Nombre: crear_pieza
* Parámetros: char tipo (carácter que identifica a la pieza), int hp (puntos de vida), int x 
    (coordenada horizontal), int y (coordenada vertical), Tablero *t (puntero al tablero)
* Retorno: Pieza * (puntero a la pieza asignada en la memoria dinámica)
* Descripción: Reserva memoria dinámica para una pieza, inicializa sus atributos (como tipo, 
    vida y coordenadas) y la posiciona en la celda correspondiente del tablero.
*/
static Pieza *crear_pieza(char tipo, int hp, int x, int y, Tablero *t){
    Pieza *p = malloc(sizeof(Pieza));
    p->tipo = tipo;
    p->hp = hp;
    p->hp_max = hp; //guarda el HP máx
    p->x = x;
    p->y = y;
    ((Celda *)t->celdas[y][x])->pieza = p;
    return p;
}

/*
* Nombre: spawn_nivel
* Parámetros: Juego *juego (puntero al estado general del juego), int nivel (nivel actual a 
    inicializar)
* Retorno: void (ninguno)
* Descripción: Genera y posiciona al Rey en una posición aleatoria en la fila inferior y 
    distribuye a los enemigos correspondientes al nivel en posiciones aleatorias válidas en las 
    filas superiores.
*/
void spawn_nivel(Juego *juego, int nivel) {
    Tablero *t = juego->t;
    
    //Inicializar lista de enemigos vacia:
    juego->enemigos.lista = NULL;
    juego->enemigos.cantidad = 0;
    juego->enemigos.capacidad = 0;

    //Rey en la fila inferior (sin esquinas):
    int rey_x = 1 + rand() % (t->W -2);
    juego->jugador = crear_pieza('R', 10, rey_x, 0, t);

    int x;

    if(nivel == 1){
        //4 peones:
        for (int i = 0; i<4; i++){
            do {x = rand() % t->W;}
            while (((Celda *)t->celdas[t->H -2][x])->pieza != NULL);
            agregar_enemigo(juego, crear_pieza('P', 1, x, t->H - 2, t));
        }

        //2 caballos:
        for (int i = 0; i<2; i++){
            do {x = rand() % t->W;}
            while (((Celda *)t->celdas[t->H -1][x])->pieza != NULL);
            agregar_enemigo(juego, crear_pieza('C', 2, x, t->H - 1, t));
        }

        //2 alfiles:
        for (int i = 0; i<2; i++){
            do {x = rand() % t->W;}
            while (((Celda *)t->celdas[t->H -1][x])->pieza != NULL);
            agregar_enemigo(juego, crear_pieza('A', 2, x, t->H - 1, t));
        }

    } else if(nivel == 2){

        //4 peones:
        for (int i = 0; i<4; i++){
            do {x = rand() % t->W;}
            while (((Celda *)t->celdas[t->H -2][x])->pieza != NULL);
            agregar_enemigo(juego, crear_pieza('P', 1, x, t->H - 2, t));
        }

        //2 caballos:
        for (int i = 0; i<2; i++){
            do {x = rand() % t->W;}
            while (((Celda *)t->celdas[t->H -1][x])->pieza != NULL);
            agregar_enemigo(juego, crear_pieza('C',2 ,x ,t->H -1 ,t));
        }

        //2 torres:
        for (int i = 0; i<2; i++){
            do {x = rand() % t->W;}
            while (((Celda *)t->celdas[t->H -1][x])->pieza != NULL);
            agregar_enemigo(juego, crear_pieza('T',4 ,x ,t->H -1 ,t));
        }
    }else if(nivel == 3){
        //2 peones:
        for (int i = 0; i<2; i++){
            do {x = rand() % t->W;}
            while (((Celda *)t->celdas[t->H -2][x])->pieza != NULL);
            agregar_enemigo(juego, crear_pieza('P',1 ,x ,t->H - 2 ,t));
        }
        //1 reina:
        x = rand() % t->W;
        agregar_enemigo(juego, crear_pieza('Q', 3, x, t->H -1, t));

        //1 alfil:
        do {x = rand() % t->W;} 
        while (((Celda *)t->celdas[t->H -1][x])->pieza != NULL);
        agregar_enemigo(juego, crear_pieza('A', 2, x, t->H -1, t));

        //1 torre:
        do {x = rand() % t->W;} 
        while (((Celda *)t->celdas[t->H -1][x])->pieza != NULL);
        agregar_enemigo(juego, crear_pieza('T', 4, x, t->H -1, t));
    }
}

/*
* Nombre: lista_enemigos_liberar
* Parámetros: ListaEnemigos *le (puntero a la estructura que contiene el arreglo dinámico de 
    piezas enemigas)
* Retorno: void (ninguno)
* Descripción: Recorre la lista de enemigos y libera la memoria (free) de cada pieza, para 
    finalmente liberar el arreglo de punteros principal y reiniciar la capacidad.
*/
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

/*
* Nombre: mover_enemigos
* Parámetros: Juego *juego (puntero al estado del juego)
* Retorno: void (ninguno)
* Descripción: Itera por cada pieza enemiga viva en el tablero y ejecuta su lógica de 
    movimiento y "pathfinding" hacia el Rey respetando las reglas de colisión y de desplazamiento 
    según el tipo de pieza (Peón, Caballo, Alfil, Torre, Reina).
*/
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
            int dy = -1; //movimiento siempre hacia abajo
            
            //Ataca al rey solo si está bajo el (ya sea diagonalmnte u ortogonalmente)
            if (ry == p->y + dy && abs(rx - p->x) <= 1) {
                //Se mueve hasta donde está el Rey (para atacarlo):
                nx = rx;
                ny = ry;
            } else {
                // Avance normal:
                ny = p->y + dy;
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

        else if (p->tipo == 'T'){
            // La Torre solo se mueve en turnos pares
            if (juego->turno_enemigos % 2 != 0) {
                // Turno impar: la Torre no se mueve, restaurar celda y continuar
                ((Celda *)t->celdas[p->y][p->x])->pieza = p;
                continue;
            }

            // Las 4 direcciones ortogonales
            int ortogonales[4][2] = {
                { 0, +1}, // arriba
                { 0, -1}, // abajo
                {-1,  0}, // izquierda
                {+1,  0}  // derecha
            };

            int mejor_x = p->x;
            int mejor_y = p->y;
            int mejor_dist = abs(rx - p->x) + abs(ry - p->y);

            for (int d = 0; d < 4; d++) {
                int dx = ortogonales[d][0];
                int dy = ortogonales[d][1];

                // Avanza hasta 3 casillas en esa dirección
                for (int paso = 1; paso <= 3; paso++) {
                    int tx = p->x + dx * paso;
                    int ty = p->y + dy * paso;

                    // Verificar límites del tablero
                    if (tx < 0 || tx >= t->W || ty < 0 || ty >= t->H) break;

                    Celda *dest = (Celda *)t->celdas[ty][tx];

                    // Si hay una pieza que no sea el Rey => camino bloqueado
                    if (dest->pieza != NULL && dest->pieza != juego->jugador) break;

                    int dist = abs(rx - tx) + abs(ry - ty);

                    if (dist < mejor_dist) {
                        mejor_dist = dist;
                        mejor_x = tx;
                        mejor_y = ty;
                    }

                    // Si llegó al Rey, parar
                    if (dest->pieza == juego->jugador) break;
                }
            }
            nx = mejor_x;
            ny = mejor_y;
        }

        else if (p->tipo == 'Q'){
            //8 direcciones: 4 ortogonales + 4 diagonales
            int direcciones[8][2] = {
                {0, +1}, { 0, -1}, {-1,  0}, {+1,  0},  // ortogonales
                {-1, +1}, {+1, +1}, {-1, -1}, {+1, -1}   // diagonales  (Alfil)
            };

            int mejor_x = p->x;
            int mejor_y = p->y;
            int mejor_dist = abs(rx - p->x) + abs(ry - p->y);

            for (int d = 0; d < 8; d++){
                int dx = direcciones[d][0];
                int dy = direcciones[d][1];

                for(int paso = 1; paso <= 4; paso++){
                    int tx = p->x + dx * paso;
                    int ty = p->y + dy * paso;

                    // Verificar límites del tablero
                    if (tx < 0 || tx >= t->W || ty < 0 || ty >= t->H) break;

                    Celda *dest = (Celda *)t->celdas[ty][tx];

                    // Si hay una pieza que no sea el Rey => camino bloqueado
                    if (dest->pieza != NULL && dest->pieza != juego->jugador) break;

                    int dist = abs(rx - tx) + abs(ry - ty);

                    if (dist < mejor_dist) {
                        mejor_dist = dist;
                        mejor_x = tx;
                        mejor_y = ty;
                    }

                    // Si llegó al Rey, parar
                    if (dest->pieza == juego->jugador) break;
                }
            }

            nx = mejor_x;
            ny = mejor_y;
        }

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
}

/*
* Nombre: verificar_estado_rey
* Parámetros: Juego *juego (puntero al estado general del juego)
* Retorno: bool (true si el Rey sigue vivo, false si fue alcanzado/capturado por un enemigo)
* Descripción: Comprueba si alguna de las piezas enemigas en el tablero logró moverse a la 
    misma casilla que ocupa el Rey actualmente.
*/
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