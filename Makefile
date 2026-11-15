CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I. -Itui

SRCS = main.c simulacion.c indices.c estructuras.c manejo_recursos.c generacion_datos.c utils.c \
	jugador.c biblioteca.c busqueda.c arbol_avl.c arbol_chaflaneado.c disolucion.c \
	tui/tui.c tui/input.c tui/widgets.c tui/ascii_art.c tui/dialogos.c tui/flujo.c

main: $(SRCS)
	$(CC) $(CFLAGS) -o main $(SRCS)

clean:
	rm -f main
