Nombre: Javiera Lizana Chartier
Rol: 202473606-k


## SUPOSICIONES:


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