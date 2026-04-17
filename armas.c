#include <stdio.h>
#include "armas.h"
#include "main.h"

bool escopeta(struct Juego *j, int dir_x, int dir_y) {
    printf("Escopeta disparada hacia direccion: %d, %d\n", dir_x, dir_y); 
    return true;
}

bool francotirador(struct Juego *j, int dir_x, int dir_y) { return true; }
bool granada(struct Juego *j, int target_x, int target_y) { return true; }
bool especial(struct Juego *j, int dir_x, int dir_y) { return true; }