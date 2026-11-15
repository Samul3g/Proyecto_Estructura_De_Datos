#ifndef TUI_H
#define TUI_H

#define TUI_RESET "\033[0m"
#define TUI_AMARILLO "\033[33m"
#define TUI_VERDE "\033[32m"
#define TUI_ROJO "\033[31m"
#define TUI_CIAN "\033[36m"
#define TUI_DIM "\033[2m"
#define TUI_NEGRITA "\033[1m"
#define TUI_TITULO "\033[1;33m"

/* 1 si stdin y stdout son una terminal.
	Entrada: ninguna
	Salida: 1 o 0 */
int tui_es_tty(void);

/* Entra al buffer alterno y al modo crudo. Si no hay TTY, no cambia la terminal.
	Entrada: ninguna
	Salida: la terminal queda lista para la interfaz */
void tui_init(void);

/* Devuelve la terminal al estado en que estaba.
	Entrada: ninguna
	Salida: cursor visible, modo canonico y buffer normal */
void tui_shutdown(void);

/* Vuelve al modo crudo despues de leer una linea.
	Entrada: ninguna
	Salida: la entrada queda sin eco y sin canon */
void tui_modo_raw(void);

/* Restaura el modo canonico para escribir una busqueda.
	Entrada: ninguna
	Salida: la entrada vuelve a mostrar lo que se escribe */
void tui_modo_linea(void);

/* Limpia el buffer alterno y deja el cursor arriba.
	Entrada: ninguna
	Salida: pantalla vacia */
void tui_cls(void);

/* Pausa la interfaz.
	Entrada: milisegundos
	Salida: el tiempo transcurrido */
void tui_msleep(int ms);

/* Imprime el texto y se detiene un momento en cada salto de linea.
	Entrada: el texto
	Salida: el texto en pantalla */
void tui_escribir(const char* texto);

#endif
