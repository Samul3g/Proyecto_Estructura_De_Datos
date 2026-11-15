#include "widgets.h"
#include "tui.h"

#include <stdio.h>

/* Titulo de una pantalla.
	Entrada: el texto
	Salida: el titulo pintado */
void widget_titulo(const char* texto) {
	printf("  %s========================================%s\n", TUI_AMARILLO, TUI_RESET);
	printf("  %s%s%s\n", TUI_TITULO, texto, TUI_RESET);
	printf("  %s========================================%s\n", TUI_AMARILLO, TUI_RESET);
}

/* Carrusel de una comuna del jugador.
	Entrada: el nombre, la satisfaccion y la necesidad
	Salida: la linea con flechas */
void widget_carrusel(const char* nombre, float satisfaccion, float necesidad) {
	printf("  %s========================================%s\n", TUI_AMARILLO, TUI_RESET);
	printf("     %s<-%s  %s%s%s\n", TUI_CIAN, TUI_RESET, TUI_NEGRITA, nombre, TUI_RESET);
	printf("         Satisf: %s%.1f%s    Necesidad: %s%.1f%s\n",
		TUI_VERDE, satisfaccion, TUI_RESET, TUI_AMARILLO, necesidad, TUI_RESET);
	printf("  %s========================================%s\n", TUI_AMARILLO, TUI_RESET);
}

/* Dos acciones, una marcada.
	Entrada: los rotulos y cual esta elegida (0 o 1)
	Salida: la barra de acciones */
void widget_acciones(const char* arriba, const char* abajo, int seleccion) {
	printf("\n");
	widget_fila(seleccion == 0, arriba, "");
	widget_fila(seleccion == 1, abajo, "");
}

/* Una fila de una lista.
	Entrada: si esta marcada, el texto y un extra a la derecha
	Salida: la fila */
void widget_fila(int marcada, const char* texto, const char* extra) {
	const char* marca = marcada ? ">" : " ";
	const char* color = marcada ? TUI_TITULO : "";
	const char* cierre = marcada ? TUI_RESET : "";
	if (extra != NULL && extra[0] != '\0') {
		printf("  %s%s%s  %s%s\n", color, marca, cierre, texto, extra);
	} else {
		printf("  %s%s%s  %s\n", color, marca, cierre, texto);
	}
}

/* Anillo de comunas. Indice humano, total, nombre e indices.
	Entrada: datos de la comuna elegida y si es del jugador
	Salida: el selector circular */
void widget_anillo(int indice, int total, const char* nombre, float necesidad, float satisfaccion, int propia) {
	printf("  %s<-%s   %s#%d%s / %d   %s%s%s",
		TUI_CIAN, TUI_RESET, TUI_NEGRITA, indice, TUI_RESET, total, TUI_NEGRITA, nombre, TUI_RESET);
	if (propia) {
		printf("  %s(tuya)%s", TUI_VERDE, TUI_RESET);
	}
	printf("   %s->%s\n", TUI_CIAN, TUI_RESET);
	printf("      necesidad %.1f    satisfaccion %.1f\n", necesidad, satisfaccion);
}

/* Barra de 20 casillas para un indice de 0 a 100.
	Entrada: la etiqueta, el valor y el color ANSI
	Salida: la barra */
void widget_barra(const char* etiqueta, float valor, const char* color) {
	int llenas = (int)(valor / 5.0f);
	if (llenas < 0) {
		llenas = 0;
	}
	if (llenas > 20) {
		llenas = 20;
	}
	printf("  %-14s %s", etiqueta, color);
	for (int i = 0; i < 20; i++) {
		putchar(i < llenas ? '#' : '-');
	}
	printf("%s %5.1f\n", TUI_RESET, valor);
}

/* Pie de pantalla.
	Entrada: el texto de ayuda
	Salida: la linea tenue */
void widget_ayuda(const char* texto) {
	printf("\n  %s%s%s\n", TUI_DIM, texto, TUI_RESET);
}

/* Marco de un regalo autonomo.
	Entrada: 1 si el destino es del jugador
	Salida: el borde de arriba */
void widget_marco_abrir(int destacado) {
	const char* color = destacado ? TUI_TITULO : TUI_DIM;
	printf("\n  %s+--------------------------------------+%s\n", color, TUI_RESET);
	if (destacado) {
		printf("  %s|  llega a una comuna tuya             |%s\n", color, TUI_RESET);
	}
}

/* Cierra el marco abierto por widget_marco_abrir.
	Entrada: 1 si iba destacado
	Salida: el borde de abajo */
void widget_marco_cerrar(int destacado) {
	const char* color = destacado ? TUI_TITULO : TUI_DIM;
	printf("  %s+--------------------------------------+%s\n", color, TUI_RESET);
}
