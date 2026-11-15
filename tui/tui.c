#define _DEFAULT_SOURCE

#include "tui.h"

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

static struct termios tui_original;
static struct termios tui_crudo;
static int tui_tty = 0;
static int tui_activo = 0;
static int tui_alt = 0;

/* 1 si stdin y stdout son una terminal.
	Entrada: ninguna
	Salida: 1 o 0 */
int tui_es_tty(void) {
	return tui_tty;
}

static void tui_restaurar_senal(int sig) {
	tui_shutdown();
	signal(sig, SIG_DFL);
	raise(sig);
}

/* Entra al buffer alterno y al modo crudo. Si no hay TTY, no cambia la terminal.
	Entrada: ninguna
	Salida: la terminal queda lista para la interfaz */
void tui_init(void) {
	if (tui_activo) {
		return;
	}

	tui_tty = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
	if (!tui_tty) {
		return;
	}

	if (tcgetattr(STDIN_FILENO, &tui_original) != 0) {
		tui_tty = 0;
		return;
	}

	tui_crudo = tui_original;
	tui_crudo.c_lflag &= ~(ICANON | ECHO);
	tui_crudo.c_cc[VMIN] = 1;
	tui_crudo.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tui_crudo);
	setvbuf(stdin, NULL, _IONBF, 0);

	printf("\033[?1049h\033[H\033[2J\033[?25l");
	fflush(stdout);
	tui_alt = 1;
	tui_activo = 1;

	struct winsize ventana;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ventana) == 0) {
		if (ventana.ws_col < 80 || ventana.ws_row < 24) {
			printf("%sLa terminal mide %dx%d. Se lee mejor con 80x24 o mas.%s\n",
				TUI_AMARILLO, ventana.ws_col, ventana.ws_row, TUI_RESET);
			fflush(stdout);
			tui_msleep(900);
		}
	}

	atexit(tui_shutdown);
	signal(SIGINT, tui_restaurar_senal);
	signal(SIGTERM, tui_restaurar_senal);
}

/* Devuelve la terminal al estado en que estaba.
	Entrada: ninguna
	Salida: cursor visible, modo canonico y buffer normal */
void tui_shutdown(void) {
	if (!tui_activo && !tui_alt) {
		return;
	}
	if (tui_alt) {
		printf("\033[?25h\033[?1049l");
		fflush(stdout);
		tui_alt = 0;
	}
	if (tui_tty) {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &tui_original);
	}
	tui_activo = 0;
}

/* Vuelve al modo crudo despues de leer una linea.
	Entrada: ninguna
	Salida: la entrada queda sin eco y sin canon */
void tui_modo_raw(void) {
	if (tui_tty) {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &tui_crudo);
	}
}

/* Restaura el modo canonico para escribir una busqueda.
	Entrada: ninguna
	Salida: la entrada vuelve a mostrar lo que se escribe */
void tui_modo_linea(void) {
	if (tui_tty) {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &tui_original);
	}
}

/* Limpia el buffer alterno y deja el cursor arriba.
	Entrada: ninguna
	Salida: pantalla vacia */
void tui_cls(void) {
	printf("\033[H\033[2J");
}

/* Pausa la interfaz.
	Entrada: milisegundos
	Salida: el tiempo transcurrido */
void tui_msleep(int ms) {
	if (ms <= 0) {
		return;
	}
	struct timespec pausa;
	pausa.tv_sec = ms / 1000;
	pausa.tv_nsec = (long)(ms % 1000) * 1000000L;
	nanosleep(&pausa, NULL);
}

/* Imprime el texto y se detiene un momento en cada salto de linea.
	Entrada: el texto
	Salida: el texto en pantalla */
void tui_escribir(const char* texto) {
	if (texto == NULL) {
		return;
	}
	while (*texto != '\0') {
		putchar(*texto);
		if (*texto == '\n') {
			fflush(stdout);
			tui_msleep(45);
		}
		texto++;
	}
	fflush(stdout);
}
