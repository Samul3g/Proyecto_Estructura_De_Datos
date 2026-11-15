
# Proyecto_Estructuras_De_Datos


Primer y segundo proyecto de Estructuras de Datos: Una sociedad solarpunk de comunas anarquistas


II semestre, 2026


Estudiantes: Samuel Ureña, Paula Flores


Profesor: Aurelio Sanabria


Comando de compilacion:

make

El mismo binario, escrito a mano:

gcc -Wall -Wextra -std=c11 -I. -Itui -o main main.c simulacion.c indices.c estructuras.c manejo_recursos.c generacion_datos.c utils.c jugador.c biblioteca.c busqueda.c arbol_avl.c arbol_chaflaneado.c disolucion.c tui/tui.c tui/input.c tui/widgets.c tui/ascii_art.c tui/dialogos.c tui/flujo.c

Comando para correr el programa:

./main

La partida pide una terminal de al menos 80x24. Todo el turno se maneja con el teclado: flechas o wasd/hjkl, Enter y Esc. Arriba y abajo recorren menus. Izquierda y derecha cambian un numero o rotan una comuna.

Los dibujos y las frases salen de tres archivos sin extension, en la carpeta del programa. Cada bloque cierra con una linea `n/`:

- `sprites_comunas`: un dibujo, una linea `n/`, otro dibujo.
- `servicios_ascii`: el nombre del bien o del servicio, el dibujo, `n/`.
- `profesiones`: `Profesion: nombre`, el sprite, `n/`, y despues `Frases:`, `Quejas:`, `Lectura:` y `Despedida:`. En una queja, `%s` se cambia por el recurso que falta.

Sin una terminal (por ejemplo, si la entrada viene de un archivo), el programa vuelve al dialogo de texto anterior.
