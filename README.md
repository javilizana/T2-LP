Nombre: Javiera Lizana Chartier
Rol: 202473606-k

## REVISAR DSP:
- ver porque los peones no se comen al rey cuando estan a los lados
- revisar movimientos peones, caballo, alfil
- revisar las vidas de las piezas
- Ver disparo de granadas en diagonales
- revisar arma especial

## ARMA ESPECIAL: Sniper 2.0

Dispara una bala en linea recta (como el sniper) con la diferencia que SI puede atravesar a las piezas y, además, su daño escala a medida que van pasando los niveles, en el nivel 1 se parte con 1 de daño, el nivel 2 y el nivel 3 hace 2 de daño (si en el nivel 3 hiciera 3 de daño mataría a la mayoría de las piezas en un solo disparom :P).

Acepta disparos en todas direcciones.


## SUPOSICIONES:
REVISAR DESPUES!!!!!
1. Como se dijo en el foro, los peones solo pueden moverse según las reglas del ajedrez, o sea, solo se puden mover VERTICALMENTE (por el eje y) acercándose hacia la parte inferior del tablero y NO puden moverse hacia arriba (la parte superior deltablero). En el caso de que el peón quede debajo del rey estos se quedarán quietos hasta que el rey vuelva a estar debajo de ellos, recién ahí podrán moverse acercandose al rey y a la parte inferior del tablero. Por ejemplo:
 5 [ ][ ][6][7][8][ ]
 4 [ ][ ][4][R][5][ ]
 3 [ ][ ][1][2][3][ ]
 2 [ ][P][ ][ ][ ][ ]
 1 [ ][ ][ ][ ][ ][ ]
 0 [ ][ ][ ][ ][ ][ ]
    0  1  2  3  4  5

Si el rey se mueve a 2, 3, 4, 5, 6, 7 u 8 el Peón se quedaría estatico hasta que el Rey este dentro del alcance del peón para matarlo (el peón mata al rey) o hasta que el rey se encuentre en la parte inferior del tablero (el peón reanuda su movimiento acercandose al rey y a la parte inferior del tablero).

1. Como se dijo en el foro, los peones solo pueden moverse según las reglas del ajedrez, o sea, solo se puden mover VERTICALMENTE (por el eje y), no se pueden mover hacia los lados.

2. colisiones: en el caso de pieza1 choque con pieza2, pieza1 se moverá hasta chocar con pieza2, en el caso de los alfiles (por ejemplo) si esq su movimiento en diagonal es bloqueado por otra pieza, este se moverá lo más posible sin saltarla, o sea, se estancará hasta que tenga alguna celda (conveniente) vacía.



## INSTRUCCIONES:
Para ejecutar:
1. Asegurarse que estén todos los archivos en la misma carpeta
2. En la terminal ejecutar: make
    Esto compilará el juego
3. En la terminal ejecutar: ./juego
    Para empezar el juego
4. Al finalizar ejecutar 'make clean' 


Fugas de memoria:
valgrind --leak-check=full ./juego









## ELIMINAR DSP!!!!!
Para hacer el tar.gz
- Desde el directorio PADRE de Tarea3LP_202473606-k crear:
tar -czf Tarea3LP_202473606-k.tar.gz Tarea3LP_202473606-k/

- verificar que este todo:
tar -tzf Tarea3LP_202473606-k.tar.gz

esto debería listar todos los archivos que estan en la carpeta# T3-LP



Para hacer push:
1. git status
    -> Esto te muestra los archivos modificados.
2. git add .
    -> Para incluir todos los archivos modificados
3. git commit -m "Descripción breve de lo que cambiaste"
    -> Crear un commit
4. git push
    -> Subir los cambios a GitHub