Nombre: Javiera Lizana Chartier
Rol: 202473606-k

## ARMA ESPECIAL: Sniper 2.0

Dispara una bala en linea recta (como el sniper) con la diferencia que SI puede atravesar a las piezas y, además, su daño escala a medida que van pasando los niveles, en el nivel 1 se parte con 1 de daño, el nivel 2 hace 2 de daño y en el nivel 3 hace 3 de daño.

El Sniper 2.0 acepta disparos en todas direcciones, además, marca con un "-"  las casillas de "recorrido" y marca con una "X" las casillas de impacto (al igual que el Sniper normal).


## SUPOSICIONES:

1. Como se dijo en el foro, los peones solo pueden moverse según las reglas del ajedrez, o sea, solo se puden mover VERTICALMENTE (por el eje y), no se pueden mover hacia los lados, la excepción de esto es al momento de atacar, los peones podrán moverse de forma diagonal u rotogonal hacia ABAJO para atacar al Rey. Por ejemplo:
 
 4 [ ][ ][ ][ ][ ]
 3 [ ][ ][ ][ ][ ]
 2 [ ][ ][P][ ][ ]
 1 [ ][1][2][3][ ]
 0 [ ][ ][ ][ ][ ]
    0  1  2  3  4 
Si el Rey está en las casillas 1, 2 o 3, el Peón podrá atacrlo y ocupar esta posicion. Si el Rey no está en ninguna de estas casillas, el Peón se moverá hasta llegar al límite inferior del tablero.


2. colisiones: en el caso de pieza1 choque con pieza2, pieza1 se moverá hasta chocar con pieza2 (a excepción de los caballos que los pueden saltar), en el caso de los alfiles (por ejemplo) si esq su movimiento en diagonal es bloqueado por otra pieza, este se moverá lo más posible sin saltarla, o sea, se estancará hasta que tenga alguna celda (conveniente) vacía.

3. En el caso de que se ingrese como input algo no válido, como por ejemplo "aaaaaaaaa", "hcsdjckh", "a ", etc. se imprimirá por pantalla un mensaje indicando que es una acción inválida y no se gastará turno, por otro lado, si se ingresa " a" (espacio + tecla válida) esto si será aceptado (no tuve tiempo de arreglarlo para que no aceptara o para que aceptara espacio + tecla válida o tecla valida + espacio :c).

4. En el caso que se dispare hacia alguno de los bordes del tablero saldrá un mensaje diciendo que el disparo estaba fuera del tablero y no consumirá turno/munición.

5. NO se implementaron lanzamientos diagonales de la Granada, esto debido a que el enunciado dice: "Se pueden eliminar los lanzamientos diagonales si así lo desean"








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